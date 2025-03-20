#include "Ressources/Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Core/GraphicsAPI/Vulkan/VulkanLogicalDevice.h"

namespace Engine
{
    namespace Resource
    {
        void Texture::Create(std::string a_path, Core::RHI::ApiInterface* a_interface, Core::RHI::IPhysicalDevice* a_physicalDevice, Core::RHI::ILogicalDevice* a_logicalDevice, Core::RHI::ICommandPool* a_commandPool)
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

            m_image = a_interface->InstantiateImage();

            const Core::RHI::ImageData t_imageData = {
                .mWidth= m_width,
                .mHeight= m_height,
                .data= m_data
            };

            m_image->Create(Core::RHI::ImageType::TEXTURE, t_imageData, a_physicalDevice, a_logicalDevice, a_commandPool);
        }

        void Texture::Destroy(Core::RHI::ILogicalDevice* a_logicalDevice)
        {
            delete m_data;
            m_image->Destroy(a_logicalDevice);
            delete m_image;
        }


    }
}
