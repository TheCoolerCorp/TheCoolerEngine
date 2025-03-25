#include "GamePlay/Camera.h"

#include "Core/Interfaces/ApiInterface.h"
#include "Core/Renderer/Renderer.h"

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

		void Camera::Create(Core::Renderer* a_renderer)
		{
			m_descriptor = a_renderer->GetInterface()->InstantiateCameraDescriptor();
			m_descriptor->Create(a_renderer->GetLogicalDevice(), a_renderer->GetPhysicalDevice(), a_renderer->GetPipeline(), a_renderer->GetCommandPool(), m_vp, 1);
		}

		void Camera::Update(const uint32_t a_frameIndex, Core::Renderer* a_renderer)
		{
			m_descriptor->Update(a_frameIndex, a_renderer->GetLogicalDevice(), m_vp.mElements.data());
		}

		void Camera::Destroy(Core::Renderer* a_renderer)
		{
			m_descriptor->Destroy(a_renderer->GetLogicalDevice());
			delete m_descriptor;
		}
	}
}
