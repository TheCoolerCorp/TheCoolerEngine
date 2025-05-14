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
			/*
			 * We update each transform recurisvely: we start with those which are at the root (parentId == -1), and then do their children recursively
			 * This allows us to automatically make any changes to the parent apply to the children, as the parent will have been update just before
			 */
			for (TransformComponent* t_component : m_components)
			{
				if (!t_component)
					continue;
				Math::Transform* t_transform = t_component->GetTransform();
				if (t_component->GetParentID() == -1) //only update the root elements
				{
					if (t_component->GetTransform()->GetNeedToUpdate()) //if we need to update it, update and forcefully update children
					{
						Math::mat4 newLocalMatrix = Math::mat4::TRS(t_transform->GetPosition(), t_transform->GetRotation(), t_transform->GetScale());

						t_component->GetTransform()->SetMatrix(newLocalMatrix);
						t_transform->SetGlobalPositionFromMatrix();
						t_transform->SetGlobalRotationFromMatrix();
						t_transform->SetGlobalScaleFromMatrix();
						t_component->GetTransform()->SetNeedToUpdate(false);

						for (int t_childId : t_component->GetChildrenIDs()) //force update on children as the parent has been modified
						{
							UpdateChild(t_childId, true);
						}
					}
					else // the children might need to be updated, so we go through them no matter what
					{
						for (int t_childId : t_component->GetChildrenIDs())
						{
							UpdateChild(t_childId);
						}
					}
				}
			}
		}

		/**
		 * updates the given child transform. If a_force is set to true, it will force an update, even if GetNeedToUpdate is false
		 * this is for when the parent is updated, and we need to update the child
		 * @param a_id the id of the child transform
		 */
		void TransformSystem::UpdateChild(int a_id, bool a_force)
		{
			if (!IsValidId(a_id))
				return;
			TransformComponent* t_component = m_components[a_id]; //get the component from the id

			if (t_component->GetParentID() == -1) //this function is only for updating children, so we make sure no edge-cases happen
				return;

			Math::Transform* t_transform = t_component->GetTransform();
			if (t_component->GetTransform()->GetNeedToUpdate() || a_force)
			{
				Math::mat4 newLocalMatrix = Math::mat4::TRS(t_transform->GetPosition(), t_transform->GetRotation(), t_transform->GetScale());
				Math::mat4 GlobalParentMatrix = GetParentsMatrix(t_component->GetParentID());
				Math::mat4 newGlobalMatrix = GlobalParentMatrix * newLocalMatrix;

				t_component->GetTransform()->SetMatrix(newGlobalMatrix);
				t_transform->SetGlobalPositionFromMatrix();
				t_transform->SetGlobalRotationFromMatrix();
				t_transform->SetGlobalScaleFromMatrix();
				t_component->GetTransform()->SetNeedToUpdate(false);

				for (int t_childId : t_component->GetChildrenIDs()) //force update on children as the parent has been modified
				{
					UpdateChild(t_childId, true);
				}
			}
			else
			{
				for (int t_childId : t_component->GetChildrenIDs()) // the children might need to be updated, so we go through them
				{
					UpdateChild(t_childId);
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
				if (!t_component)
					continue;
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
			int t_index = m_availableIds.front();
			m_availableIds.erase(m_availableIds.begin());
			m_components.at(t_index) = a_component;
			return t_index;
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
		
		
		
		