#ifndef VULKANINTERFACE_H
#define VULKANINTERFACE_H

#include "EngineExport.h"

#include "Core/Interfaces/ApiInterface.h"

#include "Core/Window/GLWindow.h"
#include "Core/GraphicsAPI/Vulkan/VulkanInstance.h"
#include "Core/GraphicsAPI/Vulkan/VulkanValidationLayers.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			class ENGINE_API VulkanInterface : public RHI::ApiInterface
			{
			public:
				~VulkanInterface() override = default;

				Window::IWindow* InstantiateWindow() override { return new Window::GLwindow; }

				RHI::IInstance* InstantiateInstance() override { return new GraphicsAPI::VulkanInstance; }

				RHI::IValidationLayers* InstantiateValidationLayers() override { return new GraphicsAPI::VulkanValidationLayers; }

			};
		}
	}
}
#endif