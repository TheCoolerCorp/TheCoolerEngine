#include "GamePlay/Systems/RenderSystem.h"

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
				t_renderObject->SetUniforms(a_renderer.GetLogicalDevice(), a_renderer.GetPhysicalDevice(), a_renderer.GetCommandPool(), a_componentsPool.GetComponent<TransformComponent>(a_componentsPool.GetIds()[i])->GetTransform()->GetModel().mElements.data(), a_renderer.GetSwapChain()->GetMaxFrame());
				t_renderObject->SetTexture(a_renderer.GetLogicalDevice(), a_componentsPool.GetComponent<MeshComponent>(a_componentsPool.GetIds()[i])->GetTexture()->GetImage(), a_renderer.GetSwapChain()->GetMaxFrame());

				m_renderDescriptors.emplace(a_componentsPool.GetIds()[i], t_renderObject);
			}
		}

		void RenderSystem::Update(ComponentsPool& a_componentsPool, Core::Renderer& a_renderer)
		{
			for (const std::vector<int>& t_ids = a_componentsPool.GetIds(); const int t_id : t_ids)
			{
				a_componentsPool.GetComponent<MeshComponent>(t_id)->Update();
				m_renderDescriptors.at(t_id)->UpdateUniforms(a_renderer.GetLogicalDevice(), a_componentsPool.GetComponent<TransformComponent>(t_id)->GetTransform()->GetModel().mElements.data(), );
			}
		}

		void RenderSystem::Render(ComponentsPool& a_componentsPool, Core::Renderer& a_renderer)
		{

		}

		void RenderSystem::End(ComponentsPool& a_componentsPool, Core::Renderer& a_renderer)
		{
			for (const std::vector<int>& t_ids = a_componentsPool.GetIds(); const int t_id : t_ids)
			{
				m_renderDescriptors.at(t_id)->Destroy(a_renderer.GetLogicalDevice());
			}
		}
	}
}
