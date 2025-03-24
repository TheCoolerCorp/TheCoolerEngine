#include "GamePlay/Camera.h"

#include "Core/Interfaces/ApiInterface.h"

namespace Engine
{
	namespace GamePlay
	{
		Camera::Camera(Math::vec3 a_up, Math::vec3 a_center, Math::vec3 a_eye, float a_fovY, float a_aspect,
			float a_near, float a_far)
		{
			a_eye.y *= -1.f;
			Math::mat4 t_proj = Math::mat4::Perspective(a_fovY, a_aspect, a_near, a_far);
			Math::mat4 t_view = Math::mat4::View(a_up, a_center, a_eye);
			m_vp = t_proj * t_view;
			m_vp.Transpose();
		}

		void Camera::Create(Core::RHI::ApiInterface* a_interface, const CameraObjectinfo& a_info)
		{
			m_descriptorPool = a_interface->InstantiateDescriptorPool();
			m_descriptorPool->Create(a_info.mLogicalDevice, 1);
			m_descriptor = a_interface->InstantiateCameraDescriptor();
			m_descriptor->Create(a_info.mLogicalDevice, a_info.mPhysicalDevice, a_info.mGraphicPipeline, m_descriptorPool, a_info.mCommandPool, m_vp);
		}

		void Camera::Update(Core::RHI::ILogicalDevice* a_logicalDevice)
		{
			m_descriptor->Update(a_logicalDevice, m_vp.mElements.data());
		}

		void Camera::Destroy(Core::RHI::ILogicalDevice* a_logicalDevice)
		{
			m_descriptor->Destroy(a_logicalDevice);
			delete m_descriptor;
			m_descriptorPool->Destroy(a_logicalDevice);
			delete m_descriptorPool;
		}
	}
}
