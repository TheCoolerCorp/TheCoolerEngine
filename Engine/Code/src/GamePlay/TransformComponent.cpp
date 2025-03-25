#include  "GamePlay/TransformComponent.h"

namespace Engine
{
	namespace GamePlay
	{
		void TransformComponent::Create()
		{
			m_transform = new Math::Transform(Math::vec3(0.f), Math::vec3(0.f), Math::vec3(1.f));
		}

		void TransformComponent::Create(Math::vec3 a_pos, Math::quat a_rot, Math::vec3 a_scale)
		{
			m_transform = new Math::Transform(a_pos, a_rot, a_scale);
		}

		void TransformComponent::Create(Math::vec3 a_pos, Math::vec3 a_rot, Math::vec3 a_scale)
		{
			m_transform = new Math::Transform(a_pos, a_rot, a_scale);
		}


		void TransformComponent::Update()
		{
			m_transform->UpdateMatrix();
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