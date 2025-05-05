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
            }

            m_width = t_texWidth;
            m_height = t_texHeight;
            m_data = t_pixels;

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
                .data = m_data
            };

            m_image->Create(Core::RHI::ImageType::TEXTURE, t_imageData, t_physicalDevice, t_logicalDevice, t_commandPool);

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

    }
}
