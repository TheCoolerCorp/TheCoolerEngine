#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "EngineExport.h"

#include "System.h"
#include "GamePlay/Components/Meshcomponent.h"
#include "Core/Renderer/Renderer.h"
#include "Core/Interfaces/IRenderObject.h"

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
			ENGINE_API void Update(Core::Renderer* a_renderer);
			ENGINE_API void Render(Core::Renderer* a_renderer);
			ENGINE_API void Destroy(Core::Renderer* a_renderer);

			ENGINE_API int AddComponent(MeshComponent* a_meshComponent);
			ENGINE_API void RemoveComponent(int a_id);

		private:
			std::vector<MeshComponent*> m_components;
			std::vector<Core::RHI::IRenderObject*> m_renderDescriptors;

			std::vector<int> m_availableIndexes;
		};
	}
}
#endif