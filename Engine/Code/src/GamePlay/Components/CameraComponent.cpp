#include "GamePlay/Components/CameraComponent.h"

#include "GamePlay/ServiceLocator.h"

#include <meta/factory.hpp>
#include <meta/meta.hpp>

namespace Engine
{
	namespace GamePlay
	{
		void CameraComponent::Register()
		{
			constexpr std::hash<std::string_view> t_hash{};

			meta::reflect<CameraSerializeData>(t_hash("CameraData"))
				.data<&CameraSerializeData::m_up>(t_hash("up"))
				.data<&CameraSerializeData::m_center>(t_hash("center"))
				.data<&CameraSerializeData::m_eye>(t_hash("eye"))
				.data<&CameraSerializeData::m_fovY>(t_hash("fovY"))
				.data<&CameraSerializeData::m_aspect>(t_hash("aspect"))
				.data<&CameraSerializeData::m_near>(t_hash("near"))
				.data<&CameraSerializeData::m_far>(t_hash("far"))
				.data<&CameraSerializeData::m_speed>(t_hash("speed"))
				.data<&CameraSerializeData::m_sensitivity>(t_hash("sensitivity"));

			meta::reflect<CameraComponent>(t_hash("CameraComponent"))
				.data<&CameraComponent::SetCameraData, &CameraComponent::GetCameraData>(t_hash("Camera"));
		}

		void CameraComponent::SetCameraData(const CameraSerializeData& a_cameraData)
		{
			/*
			 * Do not use Set
			 */	
		}

		CameraSerializeData CameraComponent::GetCameraData()
		{
			return m_camera.GetCameraData();
		}

		void CameraComponent::Create(int& a_outID)
		{
			a_outID = ServiceLocator::GetRendererSystem()->AddComponent(this);
			SetId(a_outID);
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


