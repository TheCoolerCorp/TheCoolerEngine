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
			ENGINE_API void Create(std::string a_path) override;
			ENGINE_API void Destroy() override;


		private:
			Resource::Texture* m_texture = nullptr;
		};
	}
}

#endif 