#ifndef CAMERA_H
#define CAMERA_H

#include "Math/mat4.h"

#include "EngineExport.h"
#include "Math/vec2.h"

namespace Engine
{
	namespace Core
	{
		namespace Window
		{
			class IInputHandler;
			class IWindow;
		}

		namespace RHI
		{
			class IObjectDescriptor;
		}

		class Renderer;
	}

	namespace GamePlay
	{
		struct CameraData
		{
			Math::mat4 m_vp;
			alignas(16)Math::vec3 m_pos;
		};

		struct CameraSerializeData
		{
			Math::vec3 m_up;
			Math::vec3 m_center;
			Math::vec3 m_eye;

			float m_fovY;
			float m_aspect;
			float m_near;
			float m_far;
			float m_speed;
			float m_sensitivity;
		};
		class Camera
		{
		public:
			ENGINE_API Camera() = default;
			ENGINE_API Camera(Math::vec3 a_up, Math::vec3 a_center, Math::vec3 a_eye, float a_fovY, float a_aspect, float a_near, float a_far, float a_speed, float a_sensitivity, bool a_freeCam = true);
			~Camera() = default;

			ENGINE_API void Create(Core::Renderer* a_renderer);
			ENGINE_API void Update(Core::Renderer* a_renderer, Core::Window::IInputHandler* a_inputHandler, Core::Window::IWindow* a_window, float a_deltaTime, Math::mat4 a_model = Math::mat4());
			ENGINE_API void Destroy(Core::Renderer* a_renderer);
			ENGINE_API void Set(Math::vec3 a_up, Math::vec3 a_center, Math::vec3 a_eye, float a_fovY, float a_aspect, float a_near, float a_far, float a_speed, float a_sensitivity, bool a_freeCam = true, Math::mat4 a_model = Math::mat4());
			ENGINE_API void SetFreeCam(const bool a_freeCam) { m_isFreeCam = a_freeCam; m_needToUpdate = true; }
			ENGINE_API CameraSerializeData GetCameraData() { return {m_up, m_center, m_eye, m_fovY, m_aspect, m_near, m_far, m_speed, m_sensitivity }; }
			ENGINE_API Core::RHI::IObjectDescriptor* GetDescriptor() const { return m_descriptor; }
			[[nodiscard]] ENGINE_API Math::mat4 GetViewMatrix() { return m_view; }
			[[nodiscard]] ENGINE_API Math::mat4 GetProjectionMatrix() { return m_projection; }
			[[nodiscard]] ENGINE_API bool IsFreeCam() const { return m_isFreeCam; }
		private:
			void ComputeInputs(Core::Window::IInputHandler* a_inputHandler, Core::Window::IWindow* a_window, float a_deltaTime);

			Math::mat4 m_vp;
			Math::vec3 m_up;
			Math::vec3 m_center;
			Math::vec3 m_eye;
			Math::vec3 m_baseForward;
			Math::vec3 m_currentForward;
			Math::vec3 m_right;
			Math::vec2 m_oldMousePos;
			Math::vec2 m_deltaMousePos;
			float m_fovY;
			float m_aspect;
			float m_near;
			float m_far;
			float m_speed;
			float m_worldPitch = 0;
			float m_worldYaw = 0;
			float m_sensitivity;
			bool m_needToUpdate = false;
			bool m_hasClicked = false;
			bool m_isFreeCam = true;

			Math::mat4 m_view;
			Math::mat4 m_projection;

			CameraData m_data;
			Core::RHI::IObjectDescriptor* m_descriptor{};
		};
	}
}

#endif