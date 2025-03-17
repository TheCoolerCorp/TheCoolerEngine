#ifndef IWINDOW_H
#define IWINDOW_H

#include <cstdint>

#include "EngineExport.h"


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
				ENGINE_API virtual GLwindow* CastGLFW() { return nullptr; }

				ENGINE_API virtual void Create(int a_width, int a_height) = 0;
				ENGINE_API virtual void Destroy() = 0;

				ENGINE_API virtual void PollEvents() = 0;
				ENGINE_API virtual void WaitEvents() = 0;
				ENGINE_API virtual bool ShouldClose() = 0;
				ENGINE_API virtual void GetFramebufferSize(int* a_width, int* a_height) = 0;
				ENGINE_API virtual void CreateWindowSurface(RHI::IInstance* a_instance, RHI::ISurface* a_surface) = 0;

				ENGINE_API virtual const char** GetRequiredInstanceExtensions(uint32_t* count) { (void)count; return nullptr; }

				ENGINE_API virtual bool GetResized() { return false; }
			};
		}
	}
}

#endif
