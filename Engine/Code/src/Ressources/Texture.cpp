#include "Ressources/Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Core/GraphicsAPI/Vulkan/VulkanLogicalDevice.h"

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
        }

        void Texture::Destroy()
        {
            delete m_data;
        }


    }
}
