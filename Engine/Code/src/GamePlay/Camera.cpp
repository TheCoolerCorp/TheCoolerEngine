#include "GamePlay/Camera.h"

#include "Core/Interfaces/ApiInterface.h"
#include "Core/Window/IInputHandler.h"

namespace Engine
{
	namespace GamePlay
	{
		Camera::Camera(Math::vec3 a_up, Math::vec3 a_center, Math::vec3 a_eye, float a_fovY, float a_aspect,
		               float a_near, float a_far, float a_speed) : m_up(a_up), m_center(a_center), m_eye(a_eye), m_fovY(a_fovY),
		                                            m_aspect(a_aspect), m_near(a_near), m_far(a_far), m_speed(a_speed)
		{
			const Math::mat4 t_proj = Math::mat4::Perspective(m_fovY, m_aspect, m_near, m_far);
			const Math::mat4 t_view = Math::mat4::View(m_up, m_center, m_eye);
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

		void Camera::Update(Core::RHI::ILogicalDevice* a_logicalDevice, Core::Window::IInputHandler* a_inputHandler, Core::Window::IWindow* a_window, const float a_deltaTime)
		{
			ComputeInputs(a_inputHandler, a_window, a_deltaTime);

			if (m_needToUpdate)
			{
				const Math::mat4 t_proj = Math::mat4::Perspective(m_fovY, m_aspect, m_near, m_far);
				const Math::mat4 t_view = Math::mat4::View(m_up, m_center, m_eye);
				m_vp = t_proj * t_view;
				m_vp.Transpose();
				m_needToUpdate = false;
			}

			m_descriptor->Update(a_logicalDevice, m_vp.mElements.data());
		}

		void Camera::Destroy(Core::RHI::ILogicalDevice* a_logicalDevice)
		{
			m_descriptor->Destroy(a_logicalDevice);
			delete m_descriptor;
			m_descriptorPool->Destroy(a_logicalDevice);
			delete m_descriptorPool;
		}

		void Camera::ComputeInputs(Core::Window::IInputHandler* a_inputHandler, Core::Window::IWindow* a_window, const float a_deltaTime)
		{
			const float t_deltaSpeed = m_speed * a_deltaTime;

			if (a_inputHandler->IsKeyDown(Core::Window::Key::KEY_W))
			{
				m_center.z -= t_deltaSpeed;
				m_eye.z -= t_deltaSpeed;
				m_needToUpdate = true;
			}
			if (a_inputHandler->IsKeyDown(Core::Window::Key::KEY_S))
			{
				m_center.z += t_deltaSpeed;
				m_eye.z += t_deltaSpeed;
				m_needToUpdate = true;
			}
			if (a_inputHandler->IsKeyDown(Core::Window::Key::KEY_D))
			{
				m_center.x += t_deltaSpeed;
				m_eye.x += t_deltaSpeed;
				m_needToUpdate = true;
			}
			if (a_inputHandler->IsKeyDown(Core::Window::Key::KEY_A))
			{
				m_center.x -= t_deltaSpeed;
				m_eye.x -= t_deltaSpeed;
				m_needToUpdate = true;
			}

			if (a_inputHandler->IsMouseButtonDown(Core::Window::MouseButton::MOUSE_BUTTON_RIGHT))
			{
				a_window->CaptureCursor();
				m_needToUpdate = true;
			}
			else if (a_inputHandler->IsMouseButtonReleased(Core::Window::MouseButton::MOUSE_BUTTON_RIGHT))
			{
				a_window->CaptureCursor(false);
			}
		}
	}
}
