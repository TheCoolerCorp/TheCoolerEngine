#include "../Include/Resources/ImGuiTexture.h"

#include "backends/imgui_impl_vulkan.h"
#include "Gameplay/ServiceLocator.h"
#include "Ressources/Texture.h"

namespace Editor::EditorLayer::Ui
{
	ImGuiTexture::ImGuiTexture(Engine::Core::Renderer* a_renderer, const std::string& a_path)
	{
		//we need to store a reference to the renderer
		m_renderer = a_renderer;
		Create(a_renderer, a_path);
	}

	void ImGuiTexture::Create(Engine::Core::Renderer* a_renderer, const std::string& a_path)
	{

		Engine::Resource::ResourceManager* t_resourceManager = Engine::GamePlay::ServiceLocator::GetResourceManager();

		m_texture = t_resourceManager->CreateResource<Engine::Resource::Texture>(a_path);
		if (!m_texture->IsCreated())
			m_texture->LoadAsync();
	}

	void ImGuiTexture::Destroy()
	{
		if (m_ready)
		{
			ImGui_ImplVulkan_RemoveTexture(m_dSet);
		}
	}

	void ImGuiTexture::DrawTexture(ImVec2 a_scale, bool a_centered)
	{
		if (!m_ready)
		{
			if (m_texture->IsLoaded())
			{
				m_renderer->GetLogicalDevice()->WaitIdle();
				if (!m_texture->IsCreated())
					m_texture->CreateImage(m_renderer);

				m_dSet = ImGui_ImplVulkan_AddTexture(m_texture->GetImage()->CastVulkan()->GetSampler(), m_texture->GetImage()->CastVulkan()->GetView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
				m_ready = true;
			}
			else
			{
				return;
			}
		}
		if (a_centered)
		{
			float t_width = ImGui::GetContentRegionAvail().x;
			float t_offsetx = std::max((t_width - a_scale.x) * 0.5f, 0.5f);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX()+t_offsetx);
		}
		ImGui::Image(reinterpret_cast<ImTextureID>(m_dSet), a_scale);
	}
}

