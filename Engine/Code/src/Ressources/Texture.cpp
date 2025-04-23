#include "Ressources/Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Core/GraphicsAPI/Vulkan/VulkanLogicalDevice.h"
#include "Core/Renderer/Renderer.h"

namespace Engine
{
    namespace Resource
    {
        void Texture::Create(std::string a_path)
        {
            int texWidth, texHeight, texChannels;
            stbi_uc* pixels = stbi_load(a_path.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

            if (!pixels) 
            {
                LOG_ERROR("Can't load image");
            }

            m_width = texWidth;
            m_height = texHeight;
            m_data = pixels;

            m_path = a_path;
        }

        void Texture::Destroy()
        {
            if (m_data)
            {
				delete m_data;
            }
        }

        void Texture::Load(Core::Renderer* a_renderer)
        {
            if (m_isLoaded)
            {
                return;
            }

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

            m_isLoaded = true;
            //delete m_data;
        }

        void Texture::Unload(Core::Renderer* a_renderer)
        {
            if (!m_isLoaded)
            {
                return;
            }

            Core::RHI::ILogicalDevice* t_logicalDevice = a_renderer->GetLogicalDevice();

            m_image->Destroy(t_logicalDevice);
            delete m_image;

            m_isLoaded = false;
        }

    }
}
