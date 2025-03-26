#ifndef TEXTURECOMPONENT_H
#define TEXTURECOMPONENT_H

#include "EngineExport.h"

#include "Component.h"
#include "Ressources/Texture.h"

namespace Engine
{
	namespace GamePlay
	{
		class MaterialComponent : public Component
		{
		public:
			MaterialComponent() = default;
			~MaterialComponent() override = default;

			ENGINE_API void Create();

			ENGINE_API void Update();

			ENGINE_API void Destroy();

			ENGINE_API void SetTexture(Resource::Texture* a_texture);
		private:
			Resource::Texture* m_texture = nullptr;
		};
	}
}

#endif 