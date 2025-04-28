#include  "GamePlay/Components/TransformComponent.h"

#include <meta/factory.hpp>
#include <meta/meta.hpp>

#include "Gameplay/ServiceLocator.h"

namespace Engine
{
	namespace GamePlay
	{
		void TransformComponent::Register()
		{
			constexpr std::hash<std::string_view> t_hash{};

			meta::reflect<Math::vec3>(t_hash("Vec3"))
				.data<&Math::vec3::x>(t_hash("x"))
				.data<&Math::vec3::y>(t_hash("y"))
				.data<&Math::vec3::z>(t_hash("z"));

			meta::reflect<Math::quat>(t_hash("Quat"))
				.data<&Math::quat::x>(t_hash("x"))
				.data<&Math::quat::y>(t_hash("y"))
				.data<&Math::quat::y>(t_hash("z"))
				.data<&Math::quat::z>(t_hash("w"));

			meta::reflect<TransformData>(t_hash("TransformData"))
				.data<&TransformData::mPos>(t_hash("position"))
				.data<&TransformData::mRot>(t_hash("rotation"))
				.data<&TransformData::mScale>(t_hash("scale"));

			meta::reflect<TransformComponent>(t_hash("TransformComponent"))
				.data<&TransformComponent::Set, &TransformComponent::GetTransformData>(t_hash("transform"));

			/*meta::reflect<TransformComponent>(t_hash("TransformComponent"))
				.data<&TransformComponent::GetTransformData, &TransformComponent::Set>(t_hash("transform"));

			meta::reflect<TransformComponent>(t_hash("TransformComponent"))
				.data < &TransformComponent::GetTransformData,
				[](TransformComponent& obj, const TransformData& data) {
				obj.Set(data);
				} > (t_hash("transform"));*/
		}

		ComponentType TransformComponent::Create(int& a_outId, bool a_colliderMesh)
		{
			m_transform = new Math::Transform();
			m_id = ServiceLocator::GetTransformSystem()->AddComponent(this);
			a_outId = m_id;
			return ComponentType::TRANSFORM;
		}

		void TransformComponent::Destroy()
		{
			delete m_transform;
		}


		/*void TransformComponent::Set(Math::vec3 a_pos, Math::quat a_rot, Math::vec3 a_scale)
		{
			if (m_transform->GetPosition() != a_pos)
			{
				m_transform->SetPosition(a_pos);
			}

			if (m_transform->GetRotation() != a_rot)
			{
				m_transform->SetRotation(a_rot);
			}

			if (m_transform->GetScale() != a_scale)
			{
				m_transform->SetScale(a_scale);
			}
		}

		void TransformComponent::Set(Math::vec3 a_pos, Math::vec3 a_rot, Math::vec3 a_scale)
		{
			if (m_transform->GetPosition() != a_pos)
			{
				m_transform->SetPosition(a_pos);
			}

			if (m_transform->GetRotation() != Math::quat(a_rot))
			{
				m_transform->SetRotation(a_rot);
			}

			if (m_transform->GetScale() != a_scale)
			{
				m_transform->SetScale(a_scale);
			}
		}*/

		void TransformComponent::Set(const TransformData& a_data)
		{
			const Math::vec3 t_pos = a_data.mPos;
			const Math::quat t_rot = a_data.mRot;
			const Math::vec3 t_scale = a_data.mScale;

			if (m_transform->GetPosition() != t_pos)
			{
				m_transform->SetPosition(t_pos);
			}

			if (m_transform->GetRotation() != Math::quat(t_rot))
			{
				m_transform->SetRotation(t_rot);
			}

			if (m_transform->GetScale() != t_scale)
			{
				m_transform->SetScale(t_scale);
			}
		}

		void TransformComponent::Move(Math::vec3 a_pos, Math::quat a_rot, Math::vec3 a_scale)
		{
			m_transform->Translate(a_pos);
			m_transform->Rotate(a_rot);
			m_transform->Scale(a_scale);
		}

		void TransformComponent::Move(Math::vec3 a_pos, Math::vec3 a_rot, Math::vec3 a_scale)
		{
			m_transform->Translate(a_pos);
			m_transform->Rotate(a_rot);
			m_transform->Scale(a_scale);
		}

		/**
		 * Sets a new parent Transform for this TransformComponent.
		 * Notifies the previous parent transform that this TransformComponent is no longer a child if it has one.
		 * @param a_id the id of the new parent Transform
		 */
		void TransformComponent::SetParent(int a_id)
		{
			if (a_id == m_parentId)
				return;
			//tell the previous parent we are no longer their child if we have one
			if (m_parentId != -1)
				ServiceLocator::GetTransformSystem()->GetComponent(m_parentId)->RemoveChild(m_id);
			//set the new parent and tell them we are their child
			m_parentId = a_id;
			ServiceLocator::GetTransformSystem()->GetComponent(m_parentId)->AddChild(m_id);
			m_transform->SetNeedToUpdate(true);
		}

		/**
		 * Removes the parent Transform for this TransformComponent if one is assigned.
		 */
		void TransformComponent::RemoveParent()
		{
			//tell the previous parent we are no longer their child
			if (m_parentId != -1)
			{
				ServiceLocator::GetTransformSystem()->GetComponent(m_parentId)->RemoveChild(m_id);
				m_parentId = -1;
			}
		}

		/**
		 * Adds a child Transform to this TransformComponent, and tells it of its new parent.
		 * @param a_id the id of the child Transform
		 */
		void TransformComponent::AddChild(int a_id)
		{
			//if it isnt already a child, add
			if (std::ranges::find(m_childrenIds, a_id) == m_childrenIds.end())
			{
				m_childrenIds.push_back(a_id);
				//now tell the child we are their new parent
				ServiceLocator::GetTransformSystem()->GetComponent(a_id)->SetParent(m_id);
				m_transform->SetNeedToUpdate(true);
			}
			
		}

		/**
		 * Removes the child Transform from this TransformComponent, and tells it to remove its parent.
		 * @param a_id the id of the child Transform
		 */
		void TransformComponent::RemoveChild(int a_id)
		{
			for (int a = 0; a < m_childrenIds.size(); a++)
			{
				if (m_childrenIds[a] == a_id)
				{
					m_childrenIds.erase(m_childrenIds.begin() + a);
					ServiceLocator::GetTransformSystem()->GetComponent(a_id)->RemoveParent();
					break;
				}
			}
		}

		void TransformComponent::ClearChildren()
		{
			for (int t_childId : m_childrenIds) //tell each child we are no longer their parent
			{
				ServiceLocator::GetTransformSystem()->GetComponent(t_childId)->RemoveParent();
			}
			m_childrenIds.clear();
		}

		TransformData TransformComponent::GetTransformData() const
		{
			m_transform->SetGlobalPositionFromMatrix();
			m_transform->SetGlobalRotationFromMatrix();
			m_transform->SetGlobalScaleFromMatrix();

			const Math::vec3 t_pos = m_transform->GetGlobalPosition();
			const Math::quat t_rot = m_transform->GetGlobalRotation();
			const Math::vec3 t_scale = m_transform->GetGlobalScale();

			return { .mPos= t_pos, .mRot= t_rot, .mScale= t_scale};
		}

		TransformComponent* TransformComponent::GetComponent(int a_id)
		{
			return ServiceLocator::GetTransformSystem()->GetComponent(a_id);
		}

		void TransformComponent::RemoveComponent(int a_id)
		{
			ServiceLocator::GetTransformSystem()->RemoveComponent(a_id);
		}
	}
}
