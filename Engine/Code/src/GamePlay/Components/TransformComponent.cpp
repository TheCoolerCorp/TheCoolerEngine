#include  "GamePlay/Components/TransformComponent.h"

#include "Gameplay/ServiceLocator.h"

namespace Engine
{
	namespace GamePlay
	{
		ComponentType TransformComponent::Create(uint32_t& a_outId)
		{
			m_transform = new Math::Transform();
			a_outId = ServiceLocator::GetTransformSystem()->AddTransformComponent(this);
			return ComponentType::TRANSFORM;
		}

		ComponentType TransformComponent::Create(Math::vec3 a_pos, Math::quat a_rot, Math::vec3 a_scale, uint32_t& a_outId)
		{
			m_transform = new Math::Transform(a_pos, a_rot, a_scale);
			a_outId = ServiceLocator::GetTransformSystem()->AddTransformComponent(this);
			return ComponentType::TRANSFORM;
		}

		ComponentType TransformComponent::Create(Math::vec3 a_pos, Math::vec3 a_rot, Math::vec3 a_scale, uint32_t& a_outId)
		{
			m_transform = new Math::Transform(a_pos, a_rot, a_scale);
			a_outId = ServiceLocator::GetTransformSystem()->AddTransformComponent(this);
			return ComponentType::TRANSFORM;
		}


		void TransformComponent::Update() const
		{
			m_transform->Update();
		}

		void TransformComponent::Destroy()
		{
			delete m_transform;
		}


		void TransformComponent::Set(Math::vec3 a_pos, Math::quat a_rot, Math::vec3 a_scale)
		{
			bool t_needUpdate = false;
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
	}
}
