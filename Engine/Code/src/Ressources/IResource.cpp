#include "Ressources/IResource.h"

#include "GamePlay/ServiceLocator.h"
#include "Core/Multithread/ThreadPool.h"

namespace Engine
{
	namespace Resource
	{
		void IResource::LoadAsync(Core::Renderer* a_renderer)
		{
			Core::Multithread::ThreadPool* t_threadPool =  GamePlay::ServiceLocator::GetThreadPool();
			if (t_threadPool)
			{
				auto LoadLambda = [this, a_renderer]()
					{
						this->Load(a_renderer);
					};
				t_threadPool->Enqueue(LoadLambda);
				return;
			}
			Load(a_renderer);
		}
	}
}
