#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <miniaudio.h>

#include "EngineExport.h"

namespace Engine
{
	namespace Core
	{
		namespace Audio
		{
			class SoundManager
			{
			public:
				SoundManager(const SoundManager&) = delete;
				SoundManager& operator=(const SoundManager&) = delete;

				ENGINE_API static SoundManager& Get()
				{
					static SoundManager s_soundManager{};
					return s_soundManager;
				}

				ENGINE_API void Init();
				ENGINE_API void PlaySound(const char* a_soundPath);
				ENGINE_API void UnInit();

			private:
				SoundManager() = default;
				~SoundManager() = default;

				ma_engine m_engine;
			};
		}
	}
}
#endif