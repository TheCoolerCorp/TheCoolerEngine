#include <utility>

#include "GamePlay/Others/CameraGO.h"


namespace Engine
{
	namespace GamePlay
	{
		CameraGO::CameraGO(std::string a_name)
		{
			m_name = a_name;

			AddComponent<TransformComponent>();
			m_colliderMat = GetComponent<TransformComponent>()->GetTransform()->GetTransformMatrix();

			AddComponent<CameraComponent>();
		}

		CameraGO::CameraGO(Math::vec3 a_position, Math::vec3 a_rotation, Math::vec3 a_scale, std::string a_name)
		{
			m_name = a_name;
			AddComponent<TransformComponent>();
			GetComponent<TransformComponent>()->Set({ a_position, Math::quat(a_rotation), a_scale, -1 });
			m_colliderMat = GetComponent<TransformComponent>()->GetTransform()->GetTransformMatrix();

			AddComponent<CameraComponent>();
		}

		void CameraGO::Set(Math::vec3 a_up, Math::vec3 a_center, Math::vec3 a_eye, float a_fovY, float a_aspect, float a_near, float a_far, float a_speed, float a_sensitivity)
		{
			GetComponent<CameraComponent>()->GetCamera().Set(a_up, a_center, a_eye, a_fovY, a_aspect, a_near, a_far, a_speed, a_sensitivity);
		}
		CameraGO::~CameraGO()
		{
			RemoveParent();
			ClearChildren();
		}
	}
}