#include "GamePlay/Systems/MeshRendererSystem.h"

//#include "GamePlay/Others/Camera.h"

namespace Engine
{
	namespace GamePlay
	{
		void MeshRendererSystem::Create(Core::Renderer* a_renderer)
		{
			// DO NOTHING FOR NOW
		}

		void MeshRendererSystem::Update(Core::Renderer* a_renderer, std::vector<std::pair<int, Math::mat4>> a_updatedMatrix)
		{
			Core::RHI::ILogicalDevice* t_logicalDevice = a_renderer->GetLogicalDevice();
			Core::RHI::IPhysicalDevice* t_physicalDevice = a_renderer->GetPhysicalDevice();
			Core::RHI::ISurface* t_surface = a_renderer->GetSurface();
			Core::RHI::ICommandPool* t_commandPool = a_renderer->GetCommandPool();
			Core::RHI::IGraphicPipeline* t_unlitPipeline = a_renderer->GetUnlitPipeline();
			Core::RHI::IGraphicPipeline* t_litPipeline = a_renderer->GetLitPipeline();
			int t_maxFrame = a_renderer->GetSwapChain()->GetMaxFrame();

			CreatePendingComponentsDescriptors(a_renderer->GetInterface(), t_logicalDevice, t_physicalDevice, t_surface, t_commandPool, t_unlitPipeline, t_litPipeline,t_maxFrame, a_updatedMatrix);

			for (int i = 0; i < a_updatedMatrix.size(); ++i)
			{
				m_objectsDescriptors[a_updatedMatrix[i].first]->UpdateUniforms(t_logicalDevice, 0,a_updatedMatrix[i].second.mElements.data(), 16 * sizeof(float), a_renderer->GetSwapChain()->GetCurrentFrame());
			}

		}

		void MeshRendererSystem::Destroy(Core::Renderer* a_renderer)
		{
			for (int i = 0; i < m_components.size(); ++i)
			{
				auto& comp = m_components[i];
				comp->Destroy();
				m_objectsDescriptors[i]->Destroy(a_renderer->GetLogicalDevice());
				a_renderer->GetInterface()->DestroyObjectDescriptor(m_objectsDescriptors[i]);
				delete comp;
			}
			m_components.clear();
		}

		uint32_t MeshRendererSystem::AddComponent(MeshComponent* a_meshComponent)
		{
			if (m_availableIndexes.empty())
			{
				m_components.push_back(a_meshComponent);
				m_pendingComponents.push_back((int)(m_components.size() - 1));
				return static_cast<uint32_t>(m_components.size() - 1);
			}
			for (const uint32_t t_availableIndex : m_availableIndexes)
			{
				if (m_components.at(t_availableIndex) == nullptr)
				{
					m_components.at(t_availableIndex) = a_meshComponent;
					m_pendingComponents.push_back(t_availableIndex);
					return t_availableIndex;
				}
			}
			return -1;
			// ADD PENDING INDEX
		}

		MeshComponent* MeshRendererSystem::GetComponent(uint32_t a_id)
		{
			if (a_id >= m_components.size())
			{
				return nullptr;
			}
			return m_components.at(a_id);
		}

		void MeshRendererSystem::RemoveComponent(uint32_t a_id)
		{
			if (m_components.at(a_id) != nullptr && a_id < m_components.size())
			{
				delete m_components.at(a_id);
				m_availableIndexes.push_back(a_id);
			}
		}

		void MeshRendererSystem::CreatePendingComponentsDescriptors(Core::RHI::ApiInterface* apiInterface, Core::RHI::ILogicalDevice* a_logicalDevice, Core::RHI::IPhysicalDevice* a_physicalDevice, Core::RHI::ISurface* a_surface, Core::RHI::ICommandPool* a_commandPool, Core::RHI::IGraphicPipeline* a_unlitPipeine, Core::RHI::IGraphicPipeline* a_litPipeine, uint32_t a_maxFrame, std::vector<std::pair<int, Math::mat4>>& a_updatedMatrix)
		{
			for (int i = 0; i < m_pendingComponents.size(); ++i)
			{
				Core::RHI::IObjectDescriptor* t_newRenderObject = apiInterface->InstantiateObjectDescriptor();

				Ref<Material> t_material = m_components[m_pendingComponents[i]]->GetMaterial();
				void* t_newRenderObjectMatrixData = a_updatedMatrix.at(m_pendingComponents.at(i)).second.mElements.data();

				std::vector<Core::RHI::DescriptorSetDataType> m_types;
				if (m_components.at(m_pendingComponents.at(i))->GetMaterial()->GetType() == UNLIT)
				{
					m_types = {Core::RHI::DescriptorSetDataType::DESCRIPTOR_SET_TYPE_UNIFORM_BUFFER , Core::RHI::DescriptorSetDataType::DESCRIPTOR_SET_TYPE_COMBINED_IMAGE_SAMPLER};
					t_newRenderObject->Create(a_logicalDevice, a_unlitPipeine, Core::RHI::Object, 3, 1, { a_maxFrame }, m_types);
				}
				else if (m_components.at(m_pendingComponents.at(i))->GetMaterial()->GetType() == LIT)
				{
					m_types = { Core::RHI::DescriptorSetDataType::DESCRIPTOR_SET_TYPE_UNIFORM_BUFFER, Core::RHI::DescriptorSetDataType::DESCRIPTOR_SET_TYPE_COMBINED_IMAGE_SAMPLER,
								Core::RHI::DescriptorSetDataType::DESCRIPTOR_SET_TYPE_COMBINED_IMAGE_SAMPLER, Core::RHI::DescriptorSetDataType::DESCRIPTOR_SET_TYPE_COMBINED_IMAGE_SAMPLER,
								Core::RHI::DescriptorSetDataType::DESCRIPTOR_SET_TYPE_COMBINED_IMAGE_SAMPLER, Core::RHI::DescriptorSetDataType::DESCRIPTOR_SET_TYPE_COMBINED_IMAGE_SAMPLER,
								Core::RHI::DescriptorSetDataType::DESCRIPTOR_SET_TYPE_UNIFORM_BUFFER, Core::RHI::DescriptorSetDataType::DESCRIPTOR_SET_TYPE_UNIFORM_BUFFER};

					t_newRenderObject->Create(a_logicalDevice, a_litPipeine, Core::RHI::Object, 3,3, { a_maxFrame, 1, 1 }, m_types);

					if (t_material->HasNormal())
					{
						t_newRenderObject->SetTexture(a_logicalDevice, t_material->GetNormal()->GetImage(), 2, 1);
					}
					if (t_material->HasMetallic())
					{
						t_newRenderObject->SetTexture(a_logicalDevice,  t_material->GetMetallic()->GetImage(), 3, 1);
					}
					if (t_material->HasRoughness())
					{
						t_newRenderObject->SetTexture(a_logicalDevice,  t_material->GetRoughness()->GetImage(), 4, 1);
					}
					if (t_material->HasAO())
					{
						t_newRenderObject->SetTexture(a_logicalDevice, t_material->GetAO()->GetImage(), 5, 1);
					}
					t_newRenderObject->SetUniform(a_logicalDevice, a_physicalDevice, a_commandPool, 1, &t_material->GetMaterialValues(), sizeof(MaterialValues), 6, 1);
					t_newRenderObject->SetUniform(a_logicalDevice, a_physicalDevice, a_commandPool, 2, &t_material->GetHasTextures(), sizeof(HasMaterialTextures), 7, 1);
				}
				else
				{
					LOG_ERROR("Not other type of pipeline has been implemented");
				}
				t_newRenderObject->SetUniform(a_logicalDevice, a_physicalDevice, a_commandPool, 0, t_newRenderObjectMatrixData, 16 * sizeof(float), 0, 1);

				t_newRenderObject->SetTexture(a_logicalDevice, t_material->GetAlbedo()->GetImage(), 1, 1);

				if (m_availableIndexes.empty())
				{
					m_objectsDescriptors.push_back(t_newRenderObject);
				}
				else
				{
					if (m_objectsDescriptors.at(m_availableIndexes.at(i)) == nullptr)
					{
						m_objectsDescriptors.at(m_availableIndexes.at(i)) = t_newRenderObject;
					}
				}
			}
			m_pendingComponents.clear();
		}
	}
}
