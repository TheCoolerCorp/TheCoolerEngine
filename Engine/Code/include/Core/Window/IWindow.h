#ifndef IWINDOW_H
#define IWINDOW_H

#include <cstdint>

#include "EngineExport.h"

#include "Core/Logger/Logger.h"
#include "GamePlay/Systems/EventSystem.h"

namespace Engine
{
	namespace Core
	{
		namespace RHI
		{
			class IInstance;
			class ISurface;
		}

		namespace Window
		{
			class GLwindow;

			class IWindow
			{
			public:
				ENGINE_API virtual ~IWindow() = default;
				ENGINE_API virtual GLwindow* CastGLFW() { LOG_ERROR("Can't cast"); return nullptr; }

				ENGINE_API virtual void Create(int a_width, int a_height) = 0;
				ENGINE_API virtual void Destroy() = 0;

				ENGINE_API virtual void PollEvents() = 0;
				ENGINE_API virtual void WaitEvents() = 0;
				ENGINE_API virtual bool ShouldClose() = 0;
				ENGINE_API virtual void GetFramebufferSize(int* a_width, int* a_height) = 0;
				ENGINE_API virtual void CreateWindowSurface(RHI::IInstance* a_instance, RHI::ISurface* a_surface) = 0;

				ENGINE_API virtual const char** GetRequiredInstanceExtensions(uint32_t* a_count) { (void)a_count; return nullptr; }

				ENGINE_API virtual bool GetResized() { return false; }
				ENGINE_API virtual void SetResized(bool a_resized) = 0;

				ENGINE_API virtual void ResizeFramebuffer() = 0;

				ENGINE_API virtual void CaptureCursor(bool a_capture = true) = 0;

				ENGINE_API CoolerEvent<bool>& GetMouseCapturedEvent()
				{
					return m_mouseCapturedEvent;
				}
			private:
				CoolerEvent<bool> m_mouseCapturedEvent;
			};
		}
	}
}

#endif
