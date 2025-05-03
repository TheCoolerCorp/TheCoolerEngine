#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "EngineExport.h"

#include "System.h"
#include "GamePlay/Components/Meshcomponent.h"
#include "GamePlay/Components/LightComponent.h"
#include "Core/Interfaces/IObjectDescriptor.h"

#include "Math/Transform.h"


namespace Engine
{
	namespace GamePlay
	{
		class RenderSystem : public System
		{
		public:
			ENGINE_API RenderSystem() = default;
			ENGINE_API ~RenderSystem() override = default;

			/*
			 * Init, update and destroy part of the system
			 */
			ENGINE_API void Create(Core::Renderer* a_renderer);
			ENGINE_API void Update(Core::Renderer* a_renderer, std::vector<std::pair<int, Math::UniformMatrixs>> a_updatedMatrix, std::vector<std::pair<int, Math::vec3>> a_lightsUpdate, std::vector<int> a_materialUpdate);
			ENGINE_API void Destroy(Core::Renderer* a_renderer);

			/*
			* Add component part
			* - Mesh : Straightforward, just mesh to show on the screen
			* - Light : Light in the scene to illuminate object who react to light (lit material)
			*/
			ENGINE_API int AddComponent(MeshComponent* a_meshComponent);
			ENGINE_API int AddComponent(LightComponent* a_lightComponent);
			//ENGINE_API int AddComponent(LightComponent* a_lightComponent);

			/*
			 * Remove component part
			 */
			ENGINE_API void RemoveMeshComponent(int a_id);
			ENGINE_API void RemoveLightComponent(int a_id);

			/*
			 * Mesh part getters
			 */
			std::vector<MeshComponent*>& GetMeshComponents() { return m_components; }
			ENGINE_API MeshComponent* GetMeshComponent(int a_id) const;
			std::vector<Core::RHI::IObjectDescriptor*>& GetMeshDescriptors() { return m_objectsDescriptors; }
			ENGINE_API [[nodiscard]] Core::RHI::IObjectDescriptor* GetMeshDescriptor(int a_idx);

			/*
			* Light part getters
			*/
			std::vector<LightComponent*>& GetLightComponents() { return m_lightComponents; }
			ENGINE_API LightComponent* GetLightComponent(int a_id) const;
			std::vector<Core::RHI::IObjectDescriptor*>& GetLightDescriptors() { return m_lightsDescriptors; }
			[[nodiscard]] Core::RHI::IObjectDescriptor* GetLightDescriptor(int a_idx);

			/*
			* Camera part getters
			*/
			//std::vector<MeshComponent*>& GetCameraComponents() { return m_components; }
			//std::vector<Core::RHI::IObjectDescriptor*>& GetCameraDescriptors() { return m_objectsDescriptors; }
			//[[nodiscard]] Core::RHI::IObjectDescriptor* GetCameraDescriptor(int a_idx);

		private:
			std::vector<MeshComponent*> m_components;
			std::vector<int> m_availableIndexes;
			std::vector<Core::RHI::IObjectDescriptor*> m_objectsDescriptors;
			std::vector<int> m_pendingComponents;

			std::vector<LightComponent*> m_lightComponents;
			std::vector<Core::RHI::IObjectDescriptor*> m_lightsDescriptors;
			std::vector<int> m_lightsAvailableIndexes;
			std::vector<int> m_lightsPendingComponents;

			Core::Renderer* m_renderer = nullptr;

			void CreatePendingComponentsDescriptors(Core::RHI::ApiInterface* apiInterface, Core::RHI::ILogicalDevice* a_logicalDevice,
				Core::RHI::IPhysicalDevice* a_physicalDevice, Core::RHI::ISurface* a_surface, Core::RHI::ICommandPool* a_commandPool, Core::RHI::IGraphicPipeline* a_unlitPipeine,
				Core::RHI::IGraphicPipeline* a_litPipeine, uint32_t a_maxFrame, std::vector<std::pair<int, Math::UniformMatrixs>>& a_updatedMatrix);

			void CreatePendingLightComponentsDescriptors(Core::RHI::ApiInterface* apiInterface, Core::RHI::ILogicalDevice* a_logicalDevice,
				Core::RHI::IPhysicalDevice* a_physicalDevice, Core::RHI::ISurface* a_surface, Core::RHI::ICommandPool* a_commandPool,
				Core::RHI::IGraphicPipeline* a_litPipeine, uint32_t a_maxFrame);

			void UpdateMaterial(Core::RHI::ApiInterface* apiInterface, Core::RHI::ILogicalDevice* a_logicalDevice,
				Core::RHI::IPhysicalDevice* a_physicalDevice, Core::RHI::ISurface* a_surface, Core::RHI::ICommandPool* a_commandPool, Core::RHI::IGraphicPipeline* a_unlitPipeine,
				Core::RHI::IGraphicPipeline* a_litPipeine, uint32_t a_maxFrame, std::vector<int>& a_indexes,std::vector<std::pair<int, Math::UniformMatrixs>>& a_updatedMatrix);

		};
	}
}
#endif