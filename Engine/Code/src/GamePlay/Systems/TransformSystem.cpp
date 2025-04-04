#include  "GamePlay/Systems/TransformSystem.h"

namespace Engine
{
	namespace GamePlay
	{
		void TransformSystem::Create()
		{
			
		}

		void TransformSystem::Update() const
		{
			for (const TransformComponent* t_component : m_components)
			{
				t_component->Update();
			}
		}

		void TransformSystem::Destroy()
		{
			for (const TransformComponent* t_component : m_components)
			{
				delete t_component;
			}
			m_components.clear();
			m_availableIds.clear();
		}

		uint32_t TransformSystem::AddComponent(TransformComponent* a_component)
		{
			if (m_availableIds.empty())
			{
				m_components.emplace_back(a_component);
				return static_cast<uint32_t>(m_components.size()) - 1u;
			}
			for (const uint32_t t_availableIndex : m_availableIds)
			{
				if (m_components.at(t_availableIndex) == nullptr)
				{
					m_components.at(t_availableIndex) = a_component;
					return t_availableIndex;
				}
			}
			return -1;
		}

		TransformComponent* TransformSystem::GetComponent(uint32_t a_id)
		{
			if (a_id >= m_components.size())
			{
				return nullptr;
			}
			return m_components.at(a_id);
		}

		void TransformSystem::RemoveComponent(const uint32_t a_id)
		{
			m_components[a_id]->Destroy();
			delete m_components[a_id];
			m_components[a_id] = nullptr;
			m_availableIds.push_back(a_id);
		}
		
	}
}
		
		
		
		