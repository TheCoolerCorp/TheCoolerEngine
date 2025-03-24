#ifndef  ILAYER_H
#define  ILAYER_H

namespace Editor::Core
{
	class Layer
	{
	public:
		Layer() = default;
		virtual ~Layer() = default;

		virtual void Init(Engine::Core::Window::IWindow* window, Engine::Core::Renderer* renderer) = 0;
		virtual void Update() = 0;
		virtual void Destroy() = 0;
	};
}

#endif 
