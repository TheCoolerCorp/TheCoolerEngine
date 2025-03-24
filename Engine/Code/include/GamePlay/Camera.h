#ifndef CAMERA_H
#define CAMERA_H

#include "Math/mat4.h"

#include "EngineExport.h"

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
			class ApiInterface;
			class ILogicalDevice;
			class IPhysicalDevice;
			class IGraphicPipeline;
			class IDescriptorPool;
			class ICommandPool;
		}
	}

	namespace GamePlay
	{
		struct CameraObjectinfo
		{
			Core::RHI::ILogicalDevice* mLogicalDevice;
			Core::RHI::IPhysicalDevice* mPhysicalDevice;
			Core::RHI::IGraphicPipeline* mGraphicPipeline;
			Core::RHI::ICommandPool* mCommandPool;
		};

		class Camera
		{
		public:
			Camera(Math::vec3 a_up, Math::vec3 a_center, Math::vec3 a_eye, float a_fovY, float a_aspect, float a_near, float a_far, float a_speed);
			~Camera() = default;

			ENGINE_API void Create(Core::RHI::ApiInterface* a_interface, const CameraObjectinfo& a_info);
			ENGINE_API void Update(Core::RHI::ILogicalDevice* a_logicalDevice, Core::Window::IInputHandler* a_inputHandler, Core::Window::IWindow* a_window, float a_deltaTime);
			ENGINE_API void Destroy(Core::RHI::ILogicalDevice* a_logicalDevice);

			ENGINE_API Core::RHI::ICameraDescriptor* GetDescriptor() const { return m_descriptor; }

		private:
			void ComputeInputs(Core::Window::IInputHandler* a_inputHandler, Core::Window::IWindow* a_window, float a_deltaTime);

			Math::mat4 m_vp;
			Math::vec3 m_up, m_center, m_eye;
			float m_fovY, m_aspect, m_near, m_far, m_speed;
			bool m_needToUpdate = false;

			Core::RHI::ICameraDescriptor* m_descriptor{};
			Core::RHI::IDescriptorPool* m_descriptorPool{};
		};
	}
}

#endif