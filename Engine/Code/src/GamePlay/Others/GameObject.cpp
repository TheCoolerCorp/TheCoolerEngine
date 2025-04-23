#include <utility>

#include "GamePlay/Others/GameObject.h"

#include "GamePlay/Components/Meshcomponent.h"
#include  "GamePlay/Components/MaterialComponent.h"
#include "Core/Utils.h"
namespace Engine
{
	namespace GamePlay
	{
		//std::bitset<INT32_MAX> GameObject::m_idBitset{};

		GameObject::GameObject(std::string a_name)
			:m_name(std::move(a_name))
		{
			//m_id = Utils::GenerateRandomInt(0, INT32_MAX);

			//while (m_idBitset[m_id])
			//{
			//	m_id = Utils::GenerateRandomInt(0, INT32_MAX);
			//}
			//m_idBitset.set(m_id);

			AddComponent<TransformComponent>();
		}

		GameObject::GameObject( Math::vec3 a_position, Math::vec3 a_rotation, Math::vec3 a_scale, std::string a_name)
			:m_name(std::move(a_name))
		{
			/*m_id = Utils::GenerateRandomInt(0, INT32_MAX);

			while (m_idBitset[m_id])
			{
				m_id = Utils::GenerateRandomInt(0, INT32_MAX);
			}
			m_idBitset.set(m_id);*/

			AddComponent<TransformComponent>();
		}

		GameObject::~GameObject()
		{
			RemoveParent();
			ClearChildren();
		}


		/*void GameObject::Create(Core::RHI::ApiInterface* a_interface, GameObjectinfo a_info)
		{
			m_descriptorPool = a_interface->InstantiateDescriptorPool();
			m_descriptorPool->Create(a_info.mLogicalDevice, a_info.mSize);
			m_descriptor = a_interface->InstantiateObjectDescriptor();
			m_descriptor->Create(a_info.mLogicalDevice, a_info.mPhysicalDevice, a_info.mGraphicPipeline, m_descriptorPool, a_info.mCommandPool, this, a_info.mSize);
		}

		void GameObject::Update(int a_frameIndex, Engine::Core::RHI::ILogicalDevice* a_logicalDevice)
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