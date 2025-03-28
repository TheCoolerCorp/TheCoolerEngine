#include "GamePlay/Systems/RenderSystem.h"

#include "GamePlay/Others/Camera.h"

namespace Engine
{
	namespace GamePlay
	{
		void RenderSystem::Init(ComponentsPool& a_componentsPool, Core::Renderer& a_renderer)
		{
			for (int i = 0; i < a_componentsPool.GetIds().size(); ++i)
			{
				Core::RHI::IRenderObject* t_renderObject = a_renderer.GetInterface()->InstantiateRenderObject();
				t_renderObject->Create(a_renderer.GetLogicalDevice(), a_renderer.GetPhysicalDevice(), a_renderer.GetSurface(), a_renderer.GetCommandPool(), a_renderer.GetPipeline(), a_renderer.GetSwapChain()->GetMaxFrame());
				t_renderObject->SetData(a_renderer.GetLogicalDevice(), a_renderer.GetPhysicalDevice(), a_renderer.GetCommandPool(), a_componentsPool.GetComponent<MeshComponent>(a_componentsPool.GetIds()[i])->GetTexture()->GetImage(), a_componentsPool.GetComponent<TransformComponent>(a_componentsPool.GetIds()[i])->GetTransform()->GetModel().mElements.data(), a_renderer.GetSwapChain()->GetMaxFrame());

				m_renderDescriptors.emplace(a_componentsPool.GetIds()[i], t_renderObject);
			}
		}

		void RenderSystem::Update(ComponentsPool& a_componentsPool, Core::Renderer& a_renderer)
		{
			for (const std::vector<int>& t_ids = a_componentsPool.GetIds(); const int t_id : t_ids)
			{
				a_componentsPool.GetComponent<MeshComponent>(t_id)->Update();
				m_renderDescriptors.at(t_id)->UpdateUniforms(a_renderer.GetLogicalDevice(), a_componentsPool.GetComponent<TransformComponent>(t_id)->GetTransform()->GetModel().mElements.data(), static_cast<int>(a_renderer.GetSwapChain()->GetImageIndex()));
			}
		}

		void RenderSystem::Render(ComponentsPool& a_componentsPool, Core::Renderer& a_renderer, Core::Window::IWindow* a_window, GamePlay::Camera* a_camera)
		{
			const std::vector<int>& t_ids = a_componentsPool.GetIds();
			std::unordered_map<int, Core::RHI::IBuffer*> t_vertexBuffers;
			std::unordered_map<int, Core::RHI::IBuffer*> t_indexBuffers;
			std::unordered_map<int, uint32_t> t_nbIndices;
			for (const int t_id : t_ids)
			{
				Resource::Mesh* t_mesh = a_componentsPool.GetComponent<MeshComponent>(t_id)->GetMesh();
				t_mesh->Load(&a_renderer);
				t_vertexBuffers.emplace(t_id, t_mesh->GetVertexBuffer());
				t_indexBuffers.emplace(t_id, t_mesh->GetIndexBuffer());
				t_nbIndices.emplace(t_id, t_mesh->GetNbIndices());
			}

			a_renderer.GetSwapChain()->DrawFrame(a_window, a_renderer.GetLogicalDevice(), a_renderer.GetCommandPool(), a_renderer.GetSurface(), a_renderer.GetPhysicalDevice(), a_renderer.GetRenderPass(),m_renderDescriptors, t_ids, t_vertexBuffers, t_indexBuffers, t_nbIndices, a_camera);

			t_nbIndices.clear();
			t_indexBuffers.clear();
			t_vertexBuffers.clear();
		}

		void RenderSystem::End(ComponentsPool& a_componentsPool, Core::Renderer& a_renderer)
		{
			for (const std::vector<int>& t_ids = a_componentsPool.GetIds(); const int t_id : t_ids)
			{
				Core::RHI::IRenderObject* t_renderObject = m_renderDescriptors.at(t_id);
				t_renderObject->Destroy(a_renderer.GetLogicalDevice());
				delete t_renderObject;
			}
			m_renderDescriptors.clear();
		}
	}
}
