#include "Core/Interfaces/ICommandPool.h"

namespace Engine
{
	namespace Core
	{

		namespace RHI
		{
			std::mutex ICommandPool::m_commandsMutex{};
		}
	}
}