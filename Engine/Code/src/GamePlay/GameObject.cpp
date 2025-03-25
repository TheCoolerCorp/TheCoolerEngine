#include "GamePlay/GameObject.h"

#include "GamePlay/Meshcomponent.h"
#include  "GamePlay/TextureComponent.h"
#include "Core/Utils.h"
namespace Engine
{
	namespace GamePlay
	{
		std::bitset<INT32_MAX> GameObject::m_idBitset{};

		GameObject::GameObject(Math::vec3 a_position, Math::vec3 a_rotation, Math::vec3 a_scale)
		{
			AddComponent<TransformComponent>(a_position, a_rotation, a_scale);
			
			m_id = Utils::GenerateRandomInt(0, INT32_MAX);

			while (m_idBitset[m_id])
			{
				m_id = Utils::GenerateRandomInt(0, INT32_MAX);
			}
			m_idBitset.set(m_id);
		}

		/*void GameObject::Create(Core::RHI::ApiInterface* a_interface, GameObjectinfo a_info)
		{
			m_descriptor = a_interface->InstantiateObjectDescriptor();
			m_descriptor->Create(a_info.mLogicalDevice, a_info.mPhysicalDevice, a_info.mGraphicPipeline, a_info.mDescriptorPool, a_info.mCommandPool, this, a_info.mSize);
		}

		void GameObject::Update(uint32_t a_frameIndex, Engine::Core::RHI::ILogicalDevice* a_logicalDevice)
		{
			m_transform.UpdateMatrix();
			m_descriptor->Update(a_frameIndex, a_logicalDevice, m_transform.GetModel().mElements.data());
		}
		void GameObject::Destroy(Core::RHI::ApiInterface* a_interface, Core::RHI::ILogicalDevice* a_logicalDevice)
		{
			m_descriptor->Destroy(a_logicalDevice);
			for (auto& component : m_components)
			{
				component->Destroy(a_logicalDevice);
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