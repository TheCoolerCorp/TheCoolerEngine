#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include "EngineExport.h"

#include "Component.h"
#include  "Math/Transform.h"

namespace Engine
{
	namespace GamePlay
	{
		class TransformComponent : public Component
		{
		public:
			~TransformComponent() override = default;

			ENGINE_API void Create();
			ENGINE_API void Create(Math::vec3 a_pos, Math::quat a_rot, Math::vec3 a_scale);
			ENGINE_API void Create(Math::vec3 a_pos, Math::vec3 a_rot, Math::vec3 a_scale);

			ENGINE_API void Update();

			ENGINE_API void Destroy();

			ENGINE_API void Set(Math::vec3 a_pos, Math::quat a_rot, Math::vec3 a_scale);
			ENGINE_API void Set(Math::vec3 a_pos, Math::vec3 a_rot, Math::vec3 a_scale);

			ENGINE_API void Move(Math::vec3 a_pos, Math::quat a_rot, Math::vec3 a_scale);
			ENGINE_API void Move(Math::vec3 a_pos, Math::vec3 a_rot, Math::vec3 a_scale);

		private:
			Math::Transform* m_transform = nullptr;
		};
	}
}

#endif 