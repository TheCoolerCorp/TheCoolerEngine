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
			class ICameraDescriptor;
		}

		class Renderer;
	}

	namespace GamePlay
	{
		class Camera
		{
		public:
			Camera(Math::vec3 a_up, Math::vec3 a_center, Math::vec3 a_eye, float a_fovY, float a_aspect, float a_near, float a_far, float a_speed, float a_sensitivity);
			~Camera() = default;

			ENGINE_API void Create(Core::Renderer* a_renderer);
			ENGINE_API void Update(const uint32_t a_frameIndex, Core::Renderer* a_renderer);
			ENGINE_API void Destroy(Core::Renderer* a_renderer);

			ENGINE_API Core::RHI::ICameraDescriptor* GetDescriptor() const { return m_descriptor; }

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

			Core::RHI::ICameraDescriptor* m_descriptor{};
			Core::RHI::IDescriptorPool* m_descriptorPool{};
		};
	}
}

#endif