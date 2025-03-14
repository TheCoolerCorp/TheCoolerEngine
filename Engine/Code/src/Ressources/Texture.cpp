#include "Ressources/Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Core/GraphicsAPI/Vulkan/VulkanLogicalDevice.h"

namespace Engine
{
    namespace Ressources
    {
        Texture::Texture() : m_data(new Data) {}

        void Texture::Create(std::string& a_path)
        {
            int texWidth, texHeight, texChannels;
            stbi_uc* pixels = stbi_load(a_path.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

            if (!pixels) 
            {
                LOG_ERROR("Can't load image");
            }

            m_data->mWidth = texWidth;
            m_data->mHeight = texHeight;
            m_data->mChannel = texChannels;
            m_data->mImage = pixels;
        }

        void Texture::Destroy()
        {
	        
        }


    }
}
