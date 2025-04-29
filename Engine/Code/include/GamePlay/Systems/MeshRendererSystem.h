#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "EngineExport.h"

#include "System.h"
#include "GamePlay/Components/Meshcomponent.h"
#include "Core/Renderer/Renderer.h"
#include "Core/Interfaces/IObjectDescriptor.h"
#include "Math/mat4.h"


namespace Engine
{
	namespace GamePlay
	{
		class MeshRendererSystem : public System
		{
		public:
			ENGINE_API MeshRendererSystem() = default;
			ENGINE_API ~MeshRendererSystem() override = default;

			ENGINE_API void Create(Core::Renderer* a_renderer);
			ENGINE_API void Update(Core::Renderer* a_renderer, std::vector<std::pair<int, Math::mat4>> a_updatedMatrix);
			ENGINE_API void Destroy(Core::Renderer* a_renderer);

			ENGINE_API int AddComponent(MeshComponent* a_meshComponent);
			ENGINE_API MeshComponent* GetComponent(int a_id) const;
			ENGINE_API void RemoveComponent(int a_id);

			std::vector<MeshComponent*>& GetComponents() { return m_components; }
			std::vector<Core::RHI::IObjectDescriptor*>& GetDescriptors() { return m_objectsDescriptors; }
			[[nodiscard]] Core::RHI::IObjectDescriptor* GetDescriptor(int a_idx);

		private:
			std::vector<MeshComponent*> m_components;
			std::vector<Core::RHI::IObjectDescriptor*> m_objectsDescriptors;

			std::vector<int> m_availableIndexes;

			std::vector<int> m_pendingComponents;

			void CreatePendingComponentsDescriptors(Core::RHI::ApiInterface* apiInterface, Core::RHI::ILogicalDevice* a_logicalDevice, Core::RHI::IPhysicalDevice* a_physicalDevice, Core::RHI::ISurface* a_surface, Core::RHI::ICommandPool* a_commandPool, Core::RHI::IGraphicPipeline* a_unlitPipeline, Core::RHI::IGraphicPipeline* a_litPipeine, uint32_t a_maxFrame,std::vector<std::pair<int, Math::mat4>>& a_updatedMatrix);
		};
	}
}
#endif