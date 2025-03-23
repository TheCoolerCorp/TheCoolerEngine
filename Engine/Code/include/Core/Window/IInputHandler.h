#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "EngineExport.h"

#include "IWindow.h"
#include "InputAction.h"
#include "Math/vec2.h"
namespace Engine
{
	namespace Core
	{
		namespace Window
		{
			class GLInputHandler;

			class IInputHandler
			{
			public:
				ENGINE_API virtual ~IInputHandler() = default;

				ENGINE_API virtual GLInputHandler* CastGLFW() { LOG_ERROR("Can't Cast!");return nullptr; }

				ENGINE_API virtual void Create(IWindow* a_window) = 0;
				ENGINE_API virtual void Destroy() = 0;

				ENGINE_API virtual bool IsKeyDown(Key a_key) = 0;
				ENGINE_API virtual bool IsKeyPressed(Key a_key) = 0;
				ENGINE_API virtual bool IsKeyReleased(Key a_key) = 0;

				ENGINE_API virtual bool IsMouseButtonDown(MouseButton a_mouseButton) = 0;
				ENGINE_API virtual bool IsMouseButtonPressed(MouseButton a_mouseButton) = 0;
				ENGINE_API virtual bool IsMouseButtonReleased(MouseButton a_mouseButton) = 0;
			};
		}
	}
}

#endif