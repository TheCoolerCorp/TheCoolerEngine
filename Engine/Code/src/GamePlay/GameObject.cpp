#include "GamePlay/GameObject.h"

#include "GamePlay/Meshcomponent.h"
#include  "GamePlay/TextureComponent.h"

namespace Engine
{
	namespace GamePlay
	{
		/*GameObject::GameObject(Math::vec3 a_position, Math::quat a_rotation, Math::vec3 a_scale)
		{
			m_transform = Math::Transform(a_position, a_rotation, a_scale);
		}

		void GameObject::Create(Core::RHI::ApiInterface* a_interface, GameObjectinfo a_info)
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