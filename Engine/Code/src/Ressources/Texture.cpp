#include "Ressources/Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Core/GraphicsAPI/Vulkan/VulkanLogicalDevice.h"
#include "Core/Renderer/Renderer.h"
#include "Gameplay/ServiceLocator.h"
#include "Core/Multithread/ThreadPool.h"

namespace Engine
{
    namespace Resource
    {
        void Texture::Create(const std::string a_path)
        {
            m_path = a_path;
        }

        void Texture::Destroy()
        {
	        delete m_data;
        }

        void Texture::Load()
        {
            if (m_isLoaded.load(std::memory_order_acquire) || m_isLoading.load(std::memory_order_acquire))
            {
                return;
            }

            m_isLoading.store(true, std::memory_order_release);

            int t_texWidth, t_texHeight, t_texChannels;
            stbi_uc* t_pixels = stbi_load(m_path.c_str(), &t_texWidth, &t_texHeight, &t_texChannels, STBI_rgb_alpha);
            if (!t_pixels)
            {
                LOG_ERROR("Can't load image");
                LOG_ERROR(stbi_failure_reason());
            }
            m_data = t_pixels;
            m_width = t_texWidth;
            m_height = t_texHeight;
            m_channels = 4; // Automaticely set to 4 
            m_imageType = Core::RHI::ImageType::TEXTURE;
            if (m_path.find("ao") != std::string::npos)
            {
                m_type = TextureType::RGB;
            }
            else if (m_path.find("nor") != std::string::npos || m_path.find("normal") != std::string::npos)
            {
                m_type = TextureType::RGB;
            }
            else if (m_path.find("rough") != std::string::npos || m_path.find("roughness") != std::string::npos)
            {
                m_type = TextureType::RGB;
            }
            else if (m_path.find("metal") != std::string::npos || m_path.find("metallic") != std::string::npos)
            {
                m_type = TextureType::RGB;
            }
            else if (m_path.find("cm") != std::string::npos || m_path.find("cubemap") != std::string::npos)
            {
                //int t_imageSize = (t_texWidth / 4) * (t_texHeight / 3) * 6 * 4;

                int t_tileWidth= (t_texWidth / 4);
                int t_tileHeight = (t_texHeight / 3);
                

                m_data = new unsigned char[t_tileWidth * t_tileHeight * 6 * m_channels];
                CutImage(t_texWidth, t_texHeight,t_texChannels, t_pixels,m_data);
                m_width = t_tileWidth;
                m_height = t_tileHeight;
                m_type = TextureType::RGB;
                m_imageType = Core::RHI::ImageType::CUBEMAP;
            }
            else
            {
                m_type = TextureType::SRGB;
            }
            m_isLoaded.store(true, std::memory_order_release);
            m_isLoading.store(false, std::memory_order_release);
        }

        void Texture::CreateImage(Core::Renderer* a_renderer)
        {
            if (m_isCreated.load(std::memory_order_acquire) || m_isCreating.load(std::memory_order_acquire) || !m_isLoaded.load(std::memory_order_acquire))
            {
                return;
            }

            m_isCreating.store(true, std::memory_order_release);

            Core::RHI::ApiInterface* t_interface = a_renderer->GetInterface();
            Core::RHI::IPhysicalDevice* t_physicalDevice = a_renderer->GetPhysicalDevice();
            Core::RHI::ILogicalDevice* t_logicalDevice = a_renderer->GetLogicalDevice();
            Core::RHI::ICommandPool* t_commandPool = a_renderer->GetCommandPool();

            m_image = t_interface->InstantiateImage();

            const Core::RHI::ImageData t_imageData = {
                .mWidth = m_width,
                .mHeight = m_height,
                .data = m_data,
                .channels = m_channels
            };

            switch (m_type)
            {
            case TextureType::SRGB:

				m_image->Create(m_imageType, Core::RHI::ImageFormat::FORMAT_R8G8B8A8_SRBG, t_imageData, t_physicalDevice, t_logicalDevice, t_commandPool); // Modif here
                break;
            case TextureType::RGB:
                // Normal, metallic, roughness, etc...  
                m_image->Create(m_imageType, Core::RHI::ImageFormat::FORMAT_R8G8B8A8_UNORM, t_imageData, t_physicalDevice, t_logicalDevice, t_commandPool);
                break;
            }

            //delete m_data;

            m_isCreated.store(true, std::memory_order_release);
            m_isCreating.store(false, std::memory_order_release);
        }

        void Texture::CreateImageAsync(Core::Renderer* a_renderer)
        {
            if (IsCreating() || IsCreated())
            {
                return;
            }

            Core::Multithread::ThreadPool* t_threadPool = GamePlay::ServiceLocator::GetThreadPool();
            if (t_threadPool)
            {
                auto CreateImageLambda = [this, a_renderer]()
                    {
                        this->CreateImage(a_renderer);
                    };
                t_threadPool->Enqueue(CreateImageLambda);
                return;
            }
            CreateImage(a_renderer);
        }

        void Texture::Unload(Core::Renderer* a_renderer)
        {
            if (!m_isLoaded)
            {
                return;
            }

            Core::RHI::ILogicalDevice* t_logicalDevice = a_renderer->GetLogicalDevice();

            if (m_image)
            {
                m_image->Destroy(t_logicalDevice);
                delete m_image;
            }

            m_isLoaded = false;
            m_isCreated = false;
        }

        void Texture::CutImage(int a_totalWidth, int a_totalHeight, int a_channelsCount, unsigned char* a_inData, unsigned char* a_outSortedData)
        {
            int t_facesCount = 6;
            int t_faceWidth = a_totalWidth / 4; 
            int t_faceHeight = a_totalHeight / 3;

            /*
             *      +----+
             *      | +Y |
             * +----+----+----+----+
             * | -X | +Z | +X | -Z |
             * +----+----+----+----+
             *      | -Y |
             *      +----+
             */
            std::vector<std::pair<int, int>> t_facesCoords // Order depend on graphic API (in my case Vulkan)
            {
                {2, 1}, // +X (index 0)
                {0, 1}, // -X (index 1)
                {1, 0}, // +Y (index 2)
                {1, 2}, // -Y (index 3)
                {1, 1}, // +Z (index 4)
                {3, 1}  // -Z (index 5)
            };


            for (int face = 0; face < t_facesCount; ++face)
            {
                auto [tileX, tileY] = t_facesCoords[face];

                for (int y = 0; y < t_faceHeight; ++y)
                {
                    int srcY = tileY * t_faceHeight + y;
                    int dstY = y;

                    for (int x = 0; x < t_faceWidth; ++x)
                    {
                        int srcX = tileX * t_faceWidth + x;
                        int dstX = x;

                        int srcIndex = (srcY * a_totalWidth + srcX) * a_channelsCount;
                        int dstIndex = (face * t_faceWidth * t_faceHeight + dstY * t_faceWidth + dstX) * a_channelsCount;

                        std::memcpy(&a_outSortedData[dstIndex], &a_inData[srcIndex], a_channelsCount);
                    }
                }
            }
        }

    }
}
