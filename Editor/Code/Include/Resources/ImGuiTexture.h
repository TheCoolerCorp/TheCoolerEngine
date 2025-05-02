
#ifndef IMGUITEXTURE_H
#define IMGUITEXTURE_H

#include <imgui.h>

#include "Core/Renderer/Renderer.h"
#include "Ressources/Texture.h"

namespace Editor::EditorLayer::Ui
{
	class ImGuiTexture
	{
	public:
		ImGuiTexture() = default;
		ImGuiTexture(Engine::Core::Renderer* a_renderer, const std::string& a_path);
		~ImGuiTexture() = default;
		void Create(Engine::Core::Renderer* a_renderer, const std::string& a_path);
		void Destroy();

		void DrawTexture(ImVec2 a_scale = {100.0f, 100.0f}, bool a_centered = false);
	private:
		Engine::Ref<Engine::Resource::Texture> m_texture;
		VkDescriptorSet m_dSet;
	};
}
#endif