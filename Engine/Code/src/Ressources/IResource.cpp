#include "Ressources/IResource.h"

#include "GamePlay/ServiceLocator.h"
#include "Core/Multithread/ThreadPool.h"

namespace Engine
{
	namespace Resource
	{
		void IResource::LoadAsync()
		{
			Core::Multithread::ThreadPool* t_threadPool =  GamePlay::ServiceLocator::GetThreadPool();
			if (t_threadPool)
			{
				auto LoadLambda = [this]()
					{
						this->Load();
					};
				t_threadPool->Enqueue(LoadLambda);
				return;
			}
			Load();
		}
	}
}
