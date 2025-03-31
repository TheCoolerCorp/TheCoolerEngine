#include "GamePlay/Components/ComponentsPool.h"
namespace Engine
{
	namespace GamePlay
	{
		void ComponentsPool::Destroy()
		{
			for (const int t_id : m_registerId)
			{
				m_transformComponents.at(t_id)->Destroy();
				delete m_transformComponents.at(t_id);

				m_meshesComponents.at(t_id)->Destroy();
				delete m_meshesComponents.at(t_id);
			}
		}
	}
}