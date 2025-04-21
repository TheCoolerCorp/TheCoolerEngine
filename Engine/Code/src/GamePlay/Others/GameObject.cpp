#include "GamePlay/Others/GameObject.h"

#include "GamePlay/Components/Meshcomponent.h"
#include  "GamePlay/Components/MaterialComponent.h"
#include "Core/Utils.h"
namespace Engine
{
	namespace GamePlay
	{
		//std::bitset<INT32_MAX> GameObject::m_idBitset{};

		GameObject::GameObject()
		{
			//m_id = Utils::GenerateRandomInt(0, INT32_MAX);

			//while (m_idBitset[m_id])
			//{
			//	m_id = Utils::GenerateRandomInt(0, INT32_MAX);
			//}
			//m_idBitset.set(m_id);

			AddComponent<TransformComponent>();
			m_colliderMat = GetComponent<TransformComponent>()->GetTransform()->GetTransformMatrix();
		}

		GameObject::GameObject(Math::vec3 a_position, Math::vec3 a_rotation, Math::vec3 a_scale)
		{
			/*m_id = Utils::GenerateRandomInt(0, INT32_MAX);

			while (m_idBitset[m_id])
			{
				m_id = Utils::GenerateRandomInt(0, INT32_MAX);
			}
			m_idBitset.set(m_id);*/

			AddComponent<TransformComponent>();
			m_colliderMat = GetComponent<TransformComponent>()->GetTransform()->GetTransformMatrix();
		}

		void GameObject::UpdateColliderMat()
		{
			const Physics::ColliderType t_colliderType = GetComponent<RigidBodyComponent>()->GetBodyType();
			GetComponent<TransformComponent>()->GetTransform()->SetGlobalPositionFromMatrix();
			const Math::vec3 t_pos = GetComponent<RigidBodyComponent>()->GetPos() + GetComponent<TransformComponent>()->GetTransform()->GetGlobalPosition();
			const Math::quat t_rotation = GetComponent<RigidBodyComponent>()->GetRot();

			switch (t_colliderType)
			{
			case Physics::ColliderType::BOX:
				m_colliderMat = Math::mat4::TRS(t_pos, t_rotation, GetComponent<RigidBodyComponent>()->GetBody().GetScale());
				break;
			case Physics::ColliderType::SPHERE:
				{
					const Math::vec3 t_sphereScale = Math::vec3(GetComponent<RigidBodyComponent>()->GetBody().GetRadius());
					m_colliderMat = Math::mat4::TRS(t_pos, t_rotation, t_sphereScale);
				}
				break;
			case Physics::ColliderType::CAPSULE:
				{
					const float t_xZScale = GetComponent<RigidBodyComponent>()->GetBody().GetRadius();
					const float t_yScale = GetComponent<RigidBodyComponent>()->GetBody().GetHalfHeight();
					const Math::vec3 t_capsuleScale = Math::vec3(t_xZScale, t_yScale, t_xZScale);
					m_colliderMat = Math::mat4::TRS(t_pos, t_rotation, t_capsuleScale);
				}
				break;
			}
		}


		/*void GameObject::Create(Core::RHI::ApiInterface* a_interface, GameObjectinfo a_info)
		{
			m_descriptorPool = a_interface->InstantiateDescriptorPool();
			m_descriptorPool->Create(a_info.mLogicalDevice, a_info.mSize);
			m_descriptor = a_interface->InstantiateObjectDescriptor();
			m_descriptor->Create(a_info.mLogicalDevice, a_info.mPhysicalDevice, a_info.mGraphicPipeline, m_descriptorPool, a_info.mCommandPool, this, a_info.mSize);
		}

		void GameObject::Update(uint32_t a_frameIndex, Engine::Core::RHI::ILogicalDevice* a_logicalDevice)
		{
			m_transform.UpdateMatrix();
			m_descriptor->Update(a_frameIndex, a_logicalDevice, m_transform.GetModel().mElements.data());
		}
		void GameObject::Destroy(Core::RHI::ApiInterface* a_interface, Core::RHI::ILogicalDevice* a_logicalDevice)
		{
			m_descriptor->Destroy(a_logicalDevice);
			m_descriptorPool->Destroy(a_logicalDevice);
			a_interface->DestroyObjectDescriptor(m_descriptor);
			a_interface->DestroyDescriptorPool(m_descriptorPool);
			for (const auto& t_component : m_components)
			{
				t_component->Destroy(a_logicalDevice);
			}
			m_components.clear();
		}

		GameObjectData GameObject::SubmitData()
		{
			return {
				.mVertexBuffer= GetComponent<MeshComponent>()->GetMesh()->GetVertexBuffer(),
				.mIndexBuffer= GetComponent<MeshComponent>()->GetMesh()->GetIndexBuffer(),
				.mImage= GetComponent<TextureComponent>()->GetTexture()->GetImage(),
				.mDescriptor= m_descriptor, 
				.mNbIndices= GetComponent<MeshComponent>()->GetMesh()->GetNbIndices()
			};
		}*/
	}
}