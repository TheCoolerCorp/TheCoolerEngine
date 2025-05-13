#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "EngineExport.h"

#include "System.h"
#include "GamePlay/Components/Meshcomponent.h"
#include "GamePlay/Components/LightComponent.h"
#include "GamePlay/Components/CameraComponent.h"
#include "Core/Interfaces/IObjectDescriptor.h"

#include "Math/Transform.h"


namespace Engine
{
	namespace GamePlay
	{
		#define MAX_LIGHTS uint32_t(10)
		class RenderSystem : public System
		{
		public:
			ENGINE_API RenderSystem() = default;
			ENGINE_API ~RenderSystem() override = default;

			/*
			 * Init, update and destroy part of the system
			 */
			ENGINE_API void Create(Core::Renderer* a_renderer);
			ENGINE_API void Update(Core::Renderer* a_renderer, std::vector<std::pair<int, Math::UniformMatrixs>> a_updatedMatrix, std::vector<std::tuple<int, Math::vec3, Math::vec3>> a_lightsUpdate, std::vector<int> a_materialUpdate, std::vector<std::pair<int, Math::mat4>> a_cameraUpdatedMatrix);
			ENGINE_API void UpdateCamera(Core::Renderer* a_renderer, float a_deltaTime, Core::Window::IWindow* a_window, Core::Window::IInputHandler* a_inputHandler, int a_cameraIndex, Math::mat4 a_model);
			ENGINE_API void Destroy(Core::Renderer* a_renderer);

			/*
			* Add component part
			* - Mesh : Straightforward, just mesh to show on the screen
			* - Light : Light in the scene to illuminate object who react to light (lit material)
			*/
			ENGINE_API int AddComponent(MeshComponent* a_meshComponent);
			ENGINE_API int AddComponent(LightComponent* a_lightComponent);
			ENGINE_API int AddComponent(CameraComponent* a_cameraComponent);

			/*
			 * Remove component part
			 */
			ENGINE_API void RemoveMeshComponent(int a_id);
			ENGINE_API void RemoveLightComponent(int a_id);
			ENGINE_API void RemoveCameraComponent(int a_id);

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
			[[nodiscard]] Core::RHI::IObjectDescriptor* GetLightDescriptor() { return m_lightsDescriptor; }

			/*
			* Camera part getters
			*/
			std::vector<CameraComponent*>& GetCameraComponents() { return m_cameraComponents; }
			ENGINE_API CameraComponent* GetCameraComponent(int a_id) const;
			[[nodiscard]] Core::RHI::IObjectDescriptor* GetCameraDescriptor(int a_id);

		private:
		#pragma region LIGHTS
			LightData m_lightsData[MAX_LIGHTS];
			std::vector<LightComponent*> m_lightComponents;
			Core::RHI::IObjectDescriptor* m_lightsDescriptor;
			std::vector<int> m_lightsAvailableIndexes;
			std::vector<int> m_lightsPendingComponents;
		#pragma endregion

		#pragma region MESHES
			std::vector<MeshComponent*> m_components;
			std::vector<int> m_availableIndexes;
			std::vector<Core::RHI::IObjectDescriptor*> m_objectsDescriptors;
			std::vector<int> m_pendingComponents;
		#pragma endregion

		#pragma region CAMERA
			std::vector<CameraComponent*> m_cameraComponents;
			std::vector<int> m_cameraAvailableIndexes;
			std::vector<int> m_cameraPendingComponents;
		#pragma endregion
			Core::Renderer* m_renderer = nullptr;


			void CreatePendingComponentsDescriptors(Core::Renderer* a_renderer, Core::RHI::ILogicalDevice* a_logicalDevice,
				Core::RHI::IPhysicalDevice* a_physicalDevice, Core::RHI::ISurface* a_surface, Core::RHI::ICommandPool* a_commandPool, 
				Core::RHI::IGraphicPipeline* a_unlitPipeline, Core::RHI::IGraphicPipeline* a_litPipeine, Core::RHI::IGraphicPipeline* a_skyBoxPipeline, uint32_t a_maxFrame,std::vector<std::pair<int, Math::UniformMatrixs>>& a_updatedMatrix);

			void CreatePendingLightComponentsDescriptors(Core::RHI::ApiInterface* apiInterface, Core::RHI::ILogicalDevice* a_logicalDevice,
				Core::RHI::IPhysicalDevice* a_physicalDevice, Core::RHI::ISurface* a_surface, Core::RHI::ICommandPool* a_commandPool,
				Core::RHI::IGraphicPipeline* a_litPipeine, uint32_t a_maxFrame);

			void CreatePendingCameraComponentsDescriptors(Core::RHI::ApiInterface* apiInterface, Core::RHI::ILogicalDevice* a_logicalDevice,
				Core::RHI::IPhysicalDevice* a_physicalDevice, Core::RHI::ISurface* a_surface, Core::RHI::ICommandPool* a_commandPool,
				Core::RHI::IGraphicPipeline* a_pipeline, uint32_t a_maxFrame);

			void UpdateMaterial(Core::Renderer* a_renderer, Core::RHI::ILogicalDevice* a_logicalDevice,
				Core::RHI::IPhysicalDevice* a_physicalDevice, Core::RHI::ISurface* a_surface, Core::RHI::ICommandPool* a_commandPool, Core::RHI::IGraphicPipeline* a_unlitPipeine,
				Core::RHI::IGraphicPipeline* a_litPipeine, uint32_t a_maxFrame, std::vector<int>& a_indexes,std::vector<std::pair<int, Math::UniformMatrixs>>& a_updatedMatrix);

		};
	}
}
#endif