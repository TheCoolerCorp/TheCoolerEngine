#include "../Include/Resources/ImGuiTexture.h"

#include "backends/imgui_impl_vulkan.h"
#include "Gameplay/ServiceLocator.h"
#include "Ressources/Texture.h"

namespace Editor::EditorLayer::Ui
{
	ImGuiTexture::ImGuiTexture(Engine::Core::Renderer* a_renderer, const std::string& a_path)
	{
		Create(a_renderer, a_path);
	}

	void ImGuiTexture::Create(Engine::Core::Renderer* a_renderer, const std::string& a_path)
	{

		Engine::Resource::ResourceManager* t_resourceManager = Engine::GamePlay::ServiceLocator::GetResourceManager();
		t_resourceManager->CreateResource<Engine::Resource::Texture>(a_path);

		Engine::Ref<Engine::Resource::Texture> t_texture = t_resourceManager->CreateResource<Engine::Resource::Texture>(a_path);
		t_texture->Load();
		t_texture->CreateImage(a_renderer);
		m_texture = t_texture;

		m_dSet = ImGui_ImplVulkan_AddTexture(m_texture->GetImage()->CastVulkan()->GetSampler(), m_texture->GetImage()->CastVulkan()->GetView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
	}

	void ImGuiTexture::Destroy()
	{
		ImGui_ImplVulkan_RemoveTexture(m_dSet);
	}

	void ImGuiTexture::DrawTexture(ImVec2 a_scale, bool a_centered)
	{
		if (a_centered)
		{
			float t_width = ImGui::GetContentRegionAvail().x;
			float t_offsetx = std::max((t_width - a_scale.x) * 0.5f, 0.5f);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX()+t_offsetx);
		}
		ImGui::Image(reinterpret_cast<ImTextureID>(m_dSet), a_scale);
	}
}

