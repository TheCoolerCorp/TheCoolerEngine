#include  "GamePlay/Systems/TransformSystem.h"

#include "Core/Logger/Logger.h"

namespace Engine
{
	namespace GamePlay
	{
		void TransformSystem::Create()
		{
			
		}

		void TransformSystem::Update()
		{
			for (TransformComponent* t_component : m_components)
			{
				int t_componentParentId = t_component->GetParentID();
				Math::Transform* t_transform = t_component->GetTransform();

				if (t_transform->GetNeedToUpdate())
				{
					if (t_componentParentId != -1)
					{
						Math::mat4 newLocalMatrix = Math::mat4::TRS(t_transform->GetPosition(), t_transform->GetRotation(), t_transform->GetScale());
						Math::mat4 GlobalParentMatrix = GetParentsMatrix(t_componentParentId);
						Math::mat4 newGlobalMatrix = GlobalParentMatrix * newLocalMatrix;

						t_component->GetTransform()->SetMatrix(newGlobalMatrix);
						
						continue;
					}

					Math::vec3 pos = t_transform->GetPosition();
					Math::quat rot = t_transform->GetRotation();
					Math::vec3 scale = t_transform->GetScale();
					Math::mat4 t_trs = Math::mat4::TRS(pos, rot, scale);
					t_transform->SetMatrix(t_trs);
					t_transform->SetNeedToUpdate(false);
				}
			}
		}

		Math::mat4 TransformSystem::GetParentsMatrix(int a_id)
		{
			TransformComponent* t_firstParentComp = m_components[a_id];

			if (t_firstParentComp->GetTransform()->GetNeedToUpdate())
			{
				int otherParent = m_components[a_id]->GetParentID();
				if (otherParent != -1)
				{
					t_firstParentComp->GetTransform()->SetNeedToUpdate(false);
					return t_firstParentComp->GetTransform()->GetTransformMatrix() * GetParentsMatrix(otherParent);
				}
			}
			return t_firstParentComp->GetTransform()->GetTransformMatrix();
		}

		void TransformSystem::Destroy()
		{
			for (TransformComponent* t_component : m_components)
			{
				t_component->Destroy();
				delete t_component;
			}
			m_components.clear();
			m_availableIds.clear();
		}

		int TransformSystem::AddComponent(TransformComponent* a_component)
		{
			if (m_availableIds.empty())
			{
				m_components.emplace_back(a_component);
				return static_cast<int>(m_components.size()) - 1u;
			}
			for (const int t_availableIndex : m_availableIds)
			{
				if (m_components.at(t_availableIndex) == nullptr)
				{
					m_components.at(t_availableIndex) = a_component;
					return t_availableIndex;
				}
			}
			return -1;
		}

		TransformComponent* TransformSystem::GetComponent(int a_id)
		{
			if (!IsValidId(a_id))
				return nullptr;
			if (a_id >= m_components.size())
			{
				return nullptr;
			}
			return m_components.at(a_id);
		}

		void TransformSystem::RemoveComponent(const int a_id)
		{
			if (!IsValidId(a_id))
				return;
			m_components[a_id]->Destroy();
			delete m_components[a_id];
			m_components[a_id] = nullptr;
			m_availableIds.push_back(a_id);
		}

		bool TransformSystem::IsValidId(int id)
		{
			if (id >= 0 && id < m_components.size())
			{
				if (m_components[id] != nullptr)
				{
					return true;
				}
			}
			return false;
		}
	}
}
		
		
		
		