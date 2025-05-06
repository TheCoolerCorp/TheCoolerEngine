#include "Core/Audio/SoundManager.h"

#include "Core/Logger/Logger.h"

namespace Engine
{
	namespace Core
	{
		namespace Audio
		{
			void SoundManager::Init()
			{
				const ma_result t_result = ma_engine_init(nullptr, &m_engine);
				if (t_result != MA_SUCCESS) {
					LOG_ERROR("Failed to init miniaudio engine !");
				}
			}

			void SoundManager::PlaySound(const char* a_soundPath)
			{
				ma_engine_play_sound(&m_engine, a_soundPath, nullptr);
			}

			void SoundManager::UnInit()
			{
				ma_engine_uninit(&m_engine);
			}
		}
	}
}
