#ifndef APPLICATION_H
#define APPLICATION_H

#include "EngineExport.h"

#include "GraphicsAPI/Vulkan/VulkanInterface.h"

namespace Engine
{
	namespace Core
	{
		class ENGINE_API Application
		{
		public:
			Application() = default;
			~Application() = default;

			void Create(int a_width, int a_height);
			void Run() const;
			void Destroy() const;

		private:
			RHI::ApiInterface* m_apiInterface = nullptr;
			Window::IWindow* m_window = nullptr;
		};
	}
}

#endif