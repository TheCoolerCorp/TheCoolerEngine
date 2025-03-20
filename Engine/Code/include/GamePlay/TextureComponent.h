#ifndef TEXTURECOMPONENT_H
#define TEXTURECOMPONENT_H

#include "EngineExport.h"

#include "Ressources/ResourceManager.h"
#include "Component.h"
#include "Ressources/Texture.h"

namespace Engine
{
	namespace GamePlay
	{
		class TextureComponent : public Component
		{
		public:
			ENGINE_API void Create(std::string a_path, Core::RHI::ApiInterface* a_interface, Core::RHI::IPhysicalDevice* a_physicalDevice, Core::RHI::ILogicalDevice* a_logicalDevice, Core::RHI::ICommandPool* a_commandPool) override;
			ENGINE_API void Destroy(Core::RHI::ILogicalDevice* a_logicalDevice) override;


		private:
			Resource::Texture* m_texture = nullptr;
		};
	}
}

#endif 