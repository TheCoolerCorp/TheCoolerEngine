#include "GamePlay/ComponentsBase/Camera.h"

#include "Core/Interfaces/ApiInterface.h"
#include "Core/Window/IInputHandler.h"
#include "Math/TheCoolerMath.h"
#include "Core/Renderer/Renderer.h"
#include "Core/Interfaces/IDescriptorPool.h"

namespace Engine
{
	namespace GamePlay
	{
		Camera::Camera(Math::vec3 a_up, Math::vec3 a_center, Math::vec3 a_eye, float a_fovY, float a_aspect,
			float a_near, float a_far, float a_speed, float a_sensitivity) : m_up(a_up), m_center(a_center), m_eye(a_eye), m_fovY(a_fovY),
			m_aspect(a_aspect), m_near(a_near), m_far(a_far), m_speed(a_speed), m_sensitivity(a_sensitivity)
		{
			m_baseForward = Math::vec3::Normalize(m_center - m_eye);
			m_currentForward = m_baseForward;
			m_right = Math::vec3::Normalize(Math::vec3::CrossProduct(m_currentForward, m_up));
			Math::mat4 t_proj = Math::mat4::Perspective(m_fovY, m_aspect, m_near, m_far);
			t_proj.mElements[5] *= -1.f;
			const Math::mat4 t_view = Math::mat4::View(m_up, m_center, m_eye);
			m_vp = t_proj * t_view;
			m_vp.Transpose();
			m_data.m_vp = m_vp;
			m_data.m_pos = m_eye;


		}

		void Camera::Create(Core::Renderer* a_renderer)
		{
			m_descriptor = a_renderer->GetInterface()->InstantiateObjectDescriptor();
			m_descriptor->Create(a_renderer->GetLogicalDevice(), a_renderer->GetUnlitPipeline(), Core::RHI::Camera, 1, 1, { 1 }, { {Core::RHI::DescriptorSetDataType::DESCRIPTOR_SET_TYPE_UNIFORM_BUFFER} });
			m_descriptor->SetUniform(a_renderer->GetLogicalDevice(), a_renderer->GetPhysicalDevice(), a_renderer->GetCommandPool(), 0, &m_data, sizeof(CameraData), 0, 1);
		}

		void Camera::Set(Math::vec3 a_up, Math::vec3 a_center, Math::vec3 a_eye, float a_fovY, float a_aspect, float a_near, float a_far, float a_speed, float a_sensitivity)
		{
			m_baseForward = Math::vec3::Normalize(m_center - m_eye);
			m_currentForward = m_baseForward;
			m_right = Math::vec3::Normalize(Math::vec3::CrossProduct(m_currentForward, m_up));
			Math::mat4 t_proj = Math::mat4::Perspective(m_fovY, m_aspect, m_near, m_far);
			t_proj.mElements[5] *= -1.f;
			const Math::mat4 t_view = Math::mat4::View(m_up, m_center, m_eye);
			m_vp = t_proj * t_view;
			m_vp.Transpose();
			m_data.m_vp = m_vp;
			m_data.m_pos = m_eye;
		}

		void Camera::Update(Core::Renderer* a_renderer, Core::Window::IInputHandler* a_inputHandler, Core::Window::IWindow* a_window, const float a_deltaTime)
		{
			ComputeInputs(a_inputHandler, a_window, a_deltaTime);

			int t_width, t_height;
			a_window->GetFramebufferSize(&t_width, &t_height);

			const float t_aspect = static_cast<float>(t_width) / static_cast<float>(t_height);

			if (m_needToUpdate || fabsf(t_aspect - m_aspect) > std::numeric_limits<float>::epsilon())
			{
				m_aspect = t_aspect;
				Math::mat4 t_proj = Math::mat4::Perspective(m_fovY, m_aspect, m_near, m_far);
				t_proj.mElements[5] *= -1.f;
				const Math::mat4 t_view = Math::mat4::View(m_up, m_center, m_eye);
				m_vp = t_proj * t_view;
				m_view = t_view;
				m_projection = t_proj;
				m_vp.Transpose();
				m_data.m_vp = m_vp;
				m_data.m_pos = m_eye;
				m_needToUpdate = false;
			}

			m_descriptor->UpdateUniforms(a_renderer->GetLogicalDevice(), 0, &m_data, sizeof(CameraData), 0);
		}

		void Camera::Destroy(Core::Renderer* a_renderer)
		{
			m_descriptor->Destroy(a_renderer->GetLogicalDevice());
			a_renderer->GetInterface()->DestroyObjectDescriptor(m_descriptor);
		}

		void Camera::ComputeInputs(Core::Window::IInputHandler* a_inputHandler, Core::Window::IWindow* a_window, const float a_deltaTime)
		{
			const float t_deltaSpeed = m_speed * a_deltaTime;

			if (a_inputHandler->IsKeyDown(Core::Window::Key::KEY_W))
			{
				m_center += m_currentForward * t_deltaSpeed;
				m_eye += m_currentForward * t_deltaSpeed;
				m_needToUpdate = true;
			}
			if (a_inputHandler->IsKeyDown(Core::Window::Key::KEY_S))
			{
				m_center -= m_currentForward * t_deltaSpeed;
				m_eye -= m_currentForward * t_deltaSpeed;
				m_needToUpdate = true;
			}
			if (a_inputHandler->IsKeyDown(Core::Window::Key::KEY_D))
			{
				m_center += m_right * t_deltaSpeed;
				m_eye += m_right * t_deltaSpeed;
				m_needToUpdate = true;
			}
			if (a_inputHandler->IsKeyDown(Core::Window::Key::KEY_A))
			{
				m_center -= m_right * t_deltaSpeed;
				m_eye -= m_right * t_deltaSpeed;
				m_needToUpdate = true;
			}

			if (a_inputHandler->IsMouseButtonDown(Core::Window::MouseButton::MOUSE_BUTTON_RIGHT))
			{
				a_window->CaptureCursor();
				if (!m_hasClicked)
				{
					m_oldMousePos = a_inputHandler->GetCursorPosition();
					m_hasClicked = true;
				}

				const Math::vec2 t_currentMousePos = a_inputHandler->GetCursorPosition();
				m_deltaMousePos = t_currentMousePos - m_oldMousePos;
				m_oldMousePos = t_currentMousePos;

				float t_pitch = m_deltaMousePos.y * m_sensitivity * a_deltaTime;
				float t_yaw = m_deltaMousePos.x * m_sensitivity * a_deltaTime;
				m_worldPitch -= t_pitch;
				m_worldYaw -= t_yaw;

				m_worldPitch = std::clamp(m_worldPitch, -45.f, 45.f);

				Math::mat4 t_rotationMatrix = Math::mat4::Rotation(Math::quat(Math::vec3(Math::ToRadians(m_worldPitch), Math::ToRadians(m_worldYaw), 0.f)));

				m_currentForward = t_rotationMatrix * m_baseForward;
				m_center = m_eye + m_currentForward;
				m_right = Math::vec3::Normalize(Math::vec3::CrossProduct(m_currentForward, m_up));

				m_needToUpdate = true;
			}
			else if (a_inputHandler->IsMouseButtonReleased(Core::Window::MouseButton::MOUSE_BUTTON_RIGHT))
			{
				a_window->CaptureCursor(false);
				m_hasClicked = false;
			}
		}
	}
}
