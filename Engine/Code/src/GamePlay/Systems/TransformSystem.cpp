#include  "GamePlay/Systems/TransformSystem.h"

namespace Engine
{
	namespace GamePlay
	{
		void TransformSystem::Create()
		{
			TransformComponent* t_transformComponent = new TransformComponent;
			if (!m_availableIds.empty())
			{
				m_component[0] = t_transformComponent;
				m_availableIds.erase(m_availableIds.begin());
			}
		}

		void TransformSystem::Update()
		{
			
		}

		void TransformSystem::Destroy()
		{

		}
	}
}