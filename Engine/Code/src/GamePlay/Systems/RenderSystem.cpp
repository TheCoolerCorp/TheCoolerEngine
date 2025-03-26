#include "GamePlay/Systems/RenderSystem.h"

namespace Engine
{
	namespace GamePlay
	{
		void RenderSystem::Update(ComponentsPool& a_componentsPool)
		{
			for (const std::vector<int>& t_ids = a_componentsPool.GetIds(); const int t_id : t_ids)
			{
				a_componentsPool.GetComponent<MeshComponent>(t_id)->Update();
			}
		}
	}
}
