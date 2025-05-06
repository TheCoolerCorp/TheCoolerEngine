#ifndef UIMESHCOMPONENT_H
#define UIMESHCOMPONENT_H

#include "UiInspectorComponent.h"

namespace Engine::GamePlay
{
	class MeshComponent;
}

namespace Editor::EditorLayer::Ui
{
	/*
	* The Ui Counterpart of the MeshComponent.
	* Allows the user to see but not edit the properties of the MeshComponent.
	*/
	class UiMeshComponent : public InspectorComponent
	{
		enum ImageType : std::uint8_t
		{
			ALBEDO,
			NORMAL,
			METALLIC,
			ROUGHNESS,
			AMBIENTOCCLUSION
		};

	public:
		UiMeshComponent(ImGuiLayer* a_layer, Engine::GamePlay::MeshComponent* a_meshComp)
			: InspectorComponent(a_layer), m_meshComp(a_meshComp)
		{
		}

		~UiMeshComponent() override;
		void Create() override;
		void UiDraw() override;
		void Destroy() override;

		UiComponentType GetType() override
		{
			return UiComponentType::MESH;
		}
	private:
		bool m_isOutOfDate = false;

		Engine::GamePlay::MeshComponent* m_meshComp = nullptr;

		std::unordered_map<ImageType, VkDescriptorSet> m_imageSets;
		Engine::Ref<Engine::GamePlay::Material> m_material;

		void CreateImageDescriptorSets();
		void ClearImageDescriptorSets();
		void RefreshImageDescriptorSets();

		void DrawImageInfo(ImageType a_type);
		void AddDragDropImageTarget(ImageType a_type);
		void AddDragDropMeshTarget();

		void UiDrawAlbedoInfo();
		void UiDrawMetallicInfo();
		void UiDrawRoughnessInfo();
		void UiDrawAoInfo();

		bool HasImage(ImageType a_type);

		[[nodiscard]] std::string GetPath(ImageType a_type);
		[[nodiscard]] std::string GetSize(ImageType a_type);
		[[nodiscard]] std::string ToString(ImageType a_type);
	};
}

#endif