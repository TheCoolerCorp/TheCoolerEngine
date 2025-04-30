#include <utility>

#include "GamePlay/Others/GameObject.h"

#include <ranges>

#include "GamePlay/Components/Meshcomponent.h"
#include  "GamePlay/Components/MaterialComponent.h"
#include "Core/Utils.h"
namespace Engine
{
	namespace GamePlay
	{
		GameObject::GameObject(std::string a_name)
			:m_name(std::move(a_name))
		{
			AddComponent<TransformComponent>();
			m_colliderMat = GetComponent<TransformComponent>()->GetTransform()->GetTransformMatrix();
		}

		GameObject::GameObject( Math::vec3 a_position, Math::vec3 a_rotation, Math::vec3 a_scale, std::string a_name)
			:m_name(std::move(a_name))
		{
			AddComponent<TransformComponent>();
			m_colliderMat = GetComponent<TransformComponent>()->GetTransform()->GetTransformMatrix();
		}

		void GameObject::UpdateColliderMat()
		{
			if (!GetComponent<RigidBodyComponent>())
				return;
			const Physics::ColliderType t_colliderType = GetComponent<RigidBodyComponent>()->GetColliderType();
			GetComponent<TransformComponent>()->GetTransform()->SetGlobalPositionFromMatrix();
			const Math::vec3 t_pos = GetComponent<RigidBodyComponent>()->GetPos();
			const Math::quat t_rotation = GetComponent<RigidBodyComponent>()->GetRot();

			switch (t_colliderType)
			{
			case Physics::ColliderType::BOX:
				m_colliderMat = Math::mat4::TRS(t_pos, t_rotation, GetComponent<RigidBodyComponent>()->GetBody()->GetScale());
				break;
			case Physics::ColliderType::SPHERE:
				{
					const Math::vec3 t_sphereScale = Math::vec3(GetComponent<RigidBodyComponent>()->GetBody()->GetRadius());
					m_colliderMat = Math::mat4::TRS(t_pos, t_rotation, t_sphereScale);
				}
				break;
			case Physics::ColliderType::CAPSULE:
				{
					const float t_xZScale = GetComponent<RigidBodyComponent>()->GetBody()->GetRadius();
					const float t_yScale = GetComponent<RigidBodyComponent>()->GetBody()->GetHalfHeight();
					const Math::vec3 t_capsuleScale = Math::vec3(t_xZScale, t_yScale, t_xZScale);
					m_colliderMat = Math::mat4::TRS(t_pos, t_rotation, t_capsuleScale);
				}
				break;
			}
		}

		GameObject::~GameObject()
		{
			RemoveParent();
			ClearChildren();
		}

		void GameObject::SetId(int a_id)
		{
			m_id = a_id;
			if (TransformComponent* a_component = GetComponent<TransformComponent>())
			{
				a_component->SetGameObject(m_id);
			}
		}

		void GameObject::SetParent(int a_transformId)
		{
			if (TransformComponent* a_component = GetComponent<TransformComponent>())
			{
				a_component->SetParent(a_transformId);
			}
		}

		void GameObject::RemoveParent()
		{
			if (TransformComponent* a_component = GetComponent<TransformComponent>())
			{
				a_component->RemoveParent();
			}
		}

		void GameObject::AddChild(int a_transformId)
		{
			if (TransformComponent* a_component = GetComponent<TransformComponent>())
			{
				a_component->AddChild(a_transformId);
			}
		}

		void GameObject::RemoveChild(int a_transformId)
		{
			if (TransformComponent* a_component = GetComponent<TransformComponent>())
			{
				a_component->RemoveChild(a_transformId);
			}
		}

		void GameObject::ClearChildren()
		{
			if (TransformComponent* a_component = GetComponent<TransformComponent>())
			{
				a_component->ClearChildren();
			}
		}

		int GameObject::GetParentTransformId()
		{
			if (TransformComponent* a_component = GetComponent<TransformComponent>())
			{
				return a_component->GetParentID();
			}
			return -1;
		}

		std::vector<int> GameObject::GetChildrenTransformIDs()
		{
			if (TransformComponent* a_component = GetComponent<TransformComponent>())
			{
				return a_component->GetChildrenIDs();
			}
			return std::vector<int>{};
		}


		/**
		 * returns the type of every component owned by the GameObject
		 */
		std::vector<ComponentType> GameObject::GetOwnedTypes()
		{
			std::vector<ComponentType> t_types;
			for (const auto& t_key : m_compsId | std::views::keys)
			{
				t_types.push_back(t_key);
			}
			return t_types;
		}

		bool GameObject::HasParent()
		{
			if (TransformComponent* a_component = GetComponent<TransformComponent>())
			{
				return a_component->GetParentID() != -1;
			}
			return false;
		}

		bool GameObject::HasChildren()
		{
			if (TransformComponent* a_component = GetComponent<TransformComponent>())
			{
				return !a_component->GetChildrenIDs().empty();
			}
			return false;
		}
	}
}