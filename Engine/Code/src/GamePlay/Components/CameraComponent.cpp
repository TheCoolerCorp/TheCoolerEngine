#include "GamePlay/Components/CameraComponent.h"

#include "GamePlay/ServiceLocator.h"

namespace Engine
{
	namespace GamePlay
	{
		ComponentType CameraComponent::Create(int& a_outID)
		{
			a_outID = ServiceLocator::GetRendererSystem()->AddComponent(this);
			return ComponentType::CAMERA;
		}

		void CameraComponent::SetCamera(Camera& a_camera)
		{
			m_camera = a_camera;
		}
		void CameraComponent::Destroy()
		{
			// Do nothing for now
		}

		CameraComponent* CameraComponent::GetComponent(int a_id)
		{
			return ServiceLocator::GetRendererSystem()->GetCameraComponent(a_id);
		}

		void CameraComponent::RemoveComponent(int a_id)
		{
			ServiceLocator::GetRendererSystem()->RemoveCameraComponent(a_id);
		}
	}
}


