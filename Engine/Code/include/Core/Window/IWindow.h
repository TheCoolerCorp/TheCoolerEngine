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
			class ENGINE_API GLwindow;

			class ENGINE_API IWindow
			{
			public:
				virtual ~IWindow() = default;
				virtual GLwindow* CastGLFW() { return nullptr; }

				virtual void Create(int a_width, int a_height) = 0;
				virtual void Destroy() = 0;

				virtual void PollEvents() = 0;
				virtual void WaitEvents() = 0;
				virtual bool ShouldClose() = 0;
				virtual void GetFramebufferSize(int* a_width, int* a_height) = 0;
				virtual void CreateWindowSurface(RHI::IInstance* a_instance, RHI::ISurface* a_surface) = 0;

				virtual const char** GetRequiredInstanceExtensions(uint32_t* count) { (void)count; return nullptr; }

				virtual bool GetResized() { return false; }
			};
		}
	}
}

#endif
