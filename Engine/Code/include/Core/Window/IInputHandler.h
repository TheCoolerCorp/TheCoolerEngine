#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "EngineExport.h"

#include "IWindow.h"

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

				ENGINE_API static void KeyCallBack(IWindow* a_window, int a_key, int a_scancode, int a_action, int a_mods) {}
				ENGINE_API static void MouseCallBack(IWindow* a_window, int button, int a_action, int a_mods) {}

				ENGINE_API static bool IsKeyDown(int a_key) { LOG_ERROR("Can't Cast! Default return value set to false!"); return false; }
				ENGINE_API static bool IsKeyPressed(int a_key) { LOG_ERROR("Can't Cast! Default return value set to false!"); return false; }
				ENGINE_API static bool IsKeyReleased(int a_key) { LOG_ERROR("Can't Cast! Default return value set to false!"); return false; }

				ENGINE_API static bool IsMouseButtonDown(int a_key) { LOG_ERROR("Can't Cast! Default return value set to false!"); return false; }
				ENGINE_API static bool IsMouseButtonPressed(int a_key) { LOG_ERROR("Can't Cast! Default return value set to false!"); return false; }
				ENGINE_API static bool IsMouseButtonReleased(int a_key) { LOG_ERROR("Can't Cast! Default return value set to false!"); return false; }

			protected:
				static std::vector<int> currentKeyStatus;
				static std::vector<int> currentMouseButtonStatus;
			};
		}
	}
}

#endif