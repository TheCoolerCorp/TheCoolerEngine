#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "EngineExport.h"

#include "System.h"
#include "GamePlay/Components/Meshcomponent.h"


#include "Core/Renderer/Renderer.h"
#include "Core/Interfaces/IRenderObject.h"
#include <unordered_map>

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

			/*void AddComponent(MeshComponent* a_component, int* a_gameObjectComponentId)
			{
				m_availableIndexes != 0


			}*/




		private:
			std::vector<MeshComponent*> m_components;
			std::vector<Core::RHI::IRenderObject*> m_renderDescriptors;

			std::vector<int> m_availableIndexes;
		};
		//class RenderSystem : public System
		//{
		//public:
		//	ENGINE_API RenderSystem() = default;
		//	ENGINE_API ~RenderSystem() = default;

		//	ENGINE_API void Init(ComponentsPool& a_componentsPool, Core::Renderer& a_renderer);

		//	ENGINE_API void Update(ComponentsPool& a_componentsPool, Core::Renderer& a_renderer);
		//	ENGINE_API void Render(ComponentsPool& a_componentsPool, Core::Renderer& a_renderer, Core::Window::IWindow* a_window, GamePlay::Camera* a_camera);

		//	ENGINE_API void End(ComponentsPool& a_componentsPool, Core::Renderer& a_renderer);

		//private:
		//	//std::unordered_map<int, Core::RHI::IRenderObject*> m_renderDescriptors;
		//	//void Update(ComponentsPool& a_componentsPool) override {}
		//};
	}
}
#endif