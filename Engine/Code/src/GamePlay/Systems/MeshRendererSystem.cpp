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
			Core::RHI::IGraphicPipeline* t_graphicPipeline = a_renderer->GetPipeline();
			int t_maxFrame = a_renderer->GetSwapChain()->GetMaxFrame();

			CreatePendingComponentsDescriptors(a_renderer->GetInterface(), t_logicalDevice, t_physicalDevice, t_surface, t_commandPool, t_graphicPipeline, t_maxFrame, a_updatedMatrix);

			for (int i = 0; i < a_updatedMatrix.size(); ++i)
			{
				m_renderDescriptors[a_updatedMatrix[i].first]->UpdateUniforms(t_logicalDevice, a_updatedMatrix[i].second.mElements.data(), a_renderer->GetSwapChain()->GetCurrentFrame());
			}

		}

		void MeshRendererSystem::Render(Core::Renderer* a_renderer, Core::Window::IWindow* a_window, GamePlay::Camera* a_camera)
		{
			std::vector<Core::RHI::IBuffer*> m_vertexBuffers;
			std::vector<Core::RHI::IBuffer*> m_indexesBuffers;
			std::vector<uint32_t> m_indexesCount;
			for (int i = 0; i < m_components.size(); ++i)
			{
				m_vertexBuffers.push_back(m_components.at(i)->GetMesh()->GetVertexBuffer());
				m_indexesBuffers.push_back(m_components.at(i)->GetMesh()->GetIndexBuffer());
				m_indexesCount.push_back(m_components.at(i)->GetMesh()->GetNbIndices());
			}

			a_renderer->GetSwapChain()->DrawFrame(a_window, a_renderer->GetLogicalDevice(), a_renderer->GetCommandPool(), a_renderer->GetSurface(), a_renderer->GetPhysicalDevice(), a_renderer->GetRenderPass(), m_renderDescriptors, m_vertexBuffers, m_indexesBuffers, m_indexesCount, a_camera);
		}

		void MeshRendererSystem::Destroy(Core::Renderer* a_renderer)
		{
			for (int i = 0; i < m_components.size(); ++i)
			{
				auto& comp = m_components[i];
				comp->Destroy();
				m_renderDescriptors[i]->Destroy(a_renderer->GetLogicalDevice());
				a_renderer->GetInterface()->DestroyRenderObject(m_renderDescriptors[i]);
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

		void MeshRendererSystem::CreatePendingComponentsDescriptors(Core::RHI::ApiInterface* apiInterface, Core::RHI::ILogicalDevice* a_logicalDevice, Core::RHI::IPhysicalDevice* a_physicalDevice, Core::RHI::ISurface* a_surface, Core::RHI::ICommandPool* a_commandPool, Core::RHI::IGraphicPipeline* a_graphicPipeline, int a_maxFrame, std::vector<std::pair<int, Math::mat4>>& a_updatedMatrix)
		{
			for (int i = 0; i < m_pendingComponents.size(); ++i)
			{
				Core::RHI::IRenderObject* t_newRenderObject = apiInterface->InstantiateRenderObject();

				t_newRenderObject->Create(a_logicalDevice, a_graphicPipeline, Core::RHI::Per, a_maxFrame, { Core::RHI::DescriptorSetType::DESCRIPTOR_SET_TYPE_UNIFORM_BUFFER, Core::RHI::DescriptorSetType::DESCRIPTOR_SET_TYPE_SAMPLED_IMAGE});

				Core::RHI::IImage* t_newRenderObjectTexture = m_components.at(m_pendingComponents.at(i))->GetTexture()->GetImage();
				void* t_newRenderObjectMatrixData = a_updatedMatrix.at(m_pendingComponents.at(i)).second.mElements.data();

				t_newRenderObject->SetTexture(a_logicalDevice, t_newRenderObjectTexture, 1, 1);
				t_newRenderObject->SetMat(a_logicalDevice, a_physicalDevice, a_commandPool, t_newRenderObjectMatrixData, 0, 1);

				if (m_availableIndexes.empty())
				{
					m_renderDescriptors.push_back(t_newRenderObject);
				}
				else
				{
					if (m_renderDescriptors.at(m_availableIndexes.at(i)) == nullptr)
					{
						m_renderDescriptors.at(m_availableIndexes.at(i)) = t_newRenderObject;
					}
				}
			}
			m_pendingComponents.clear();
		}
	}
}
