#include "GamePlay/ComponentsPool.h"
namespace Engine
{
	namespace GamePlay
	{
		void ComponentsPool::RegisterTransform(int a_id, TransformComponent* a_transformComponent)
		{
			if (m_transformComponents.find(a_id) != m_transformComponents.end())
			{
				// TODO : rework warning message later
				LOG_WARNING("Cant add this component, game object Already have one");
				return;
			}
			m_transformComponents.at(a_id) = a_transformComponent;

			auto it = std::find(m_registerId.begin(), m_registerId.end(), a_id);
			if (it != m_registerId.end())
			{
				return;
			}
			m_registerId.push_back(a_id);
		}

		void ComponentsPool::RegisterMesh(int a_id, MeshComponent* a_meshComponent)
		{
			if (m_meshesComponents.find(a_id) != m_meshesComponents.end())
			{
				// TODO : rework warning message later
				LOG_WARNING("Cant add this component, game object Already have one");
				return;
			}
			m_meshesComponents.at(a_id) = a_meshComponent;

			auto it = std::find(m_registerId.begin(), m_registerId.end(), a_id);
			if (it != m_registerId.end())
			{
				return;
			}
			m_registerId.push_back(a_id);
		}
	}
}