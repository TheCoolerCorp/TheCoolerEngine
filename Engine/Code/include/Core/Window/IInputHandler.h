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

				ENGINE_API virtual bool IsKeyDown(int a_key) = 0;
				ENGINE_API virtual bool IsKeyPressed(int a_key) = 0;
				ENGINE_API virtual bool IsKeyReleased(int a_key) = 0;

				ENGINE_API virtual bool IsMouseButtonDown(int a_key) = 0;
				ENGINE_API virtual bool IsMouseButtonPressed(int a_key) = 0;
				ENGINE_API virtual bool IsMouseButtonReleased(int a_key) = 0;

				//ENGINE_API virtual bool IsMouseButtonReleased(int a_key) { LOG_ERROR("Can't Cast! Default return value set to false!"); return false; }
				//ENGINE_API virtual bool IsMouseButtonReleased(int a_key) { LOG_ERROR("Can't Cast! Default return value set to false!"); return false; }

			protected:
				static std::vector<int> currentKeyStatus;
				static std::vector<int> currentMouseButtonStatus;
			};
		}
	}
}

#endif