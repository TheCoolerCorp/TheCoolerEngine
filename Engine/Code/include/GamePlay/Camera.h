#ifndef CAMERA_H
#define CAMERA_H

#include "Math/mat4.h"

#include "EngineExport.h"

namespace Engine
{
	namespace Core
	{
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
			Camera(Math::vec3 a_up, Math::vec3 a_center, Math::vec3 a_eye, float a_fovY, float a_aspect, float a_near, float a_far);
			~Camera() = default;

			ENGINE_API void Create(Core::RHI::ApiInterface* a_interface, const CameraObjectinfo& a_info);
			ENGINE_API void Update(Core::RHI::ILogicalDevice* a_logicalDevice);
			ENGINE_API void Destroy(Core::RHI::ILogicalDevice* a_logicalDevice);

			ENGINE_API Core::RHI::ICameraDescriptor* GetDescriptor() const { return m_descriptor; }

		private:
			Math::mat4 m_vp;

			Core::RHI::ICameraDescriptor* m_descriptor{};
			Core::RHI::IDescriptorPool* m_descriptorPool{};
		};
	}
}

#endif