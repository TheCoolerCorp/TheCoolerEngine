#ifndef LAYERBASE_H
#define LAYERBASE_H
#include <string>


namespace Engine::Core
{
	class Renderer;
}

/*
* Layers are used to add functionality to the editor, like ui, audio, etc.
* Each layer should be self-contained and should not depend on other layers.
* Removing/Adding a layer should not affect the rest of the editor and not prevent
* the application from running. It is allowed to communicate with the Engine but
* the engine is not.
*/
class Layer {
public:
    Layer(const std::string& name = "UnnamedLayer", Engine::Core::Renderer* renderer) : m_name(name) {}
    virtual ~Layer() = default;

    // Called once when the layer is added
    virtual void OnAttach(Engine::Core::Window::IWindow* window) {}

    // Called once when the layer is removed
    virtual void OnDetach() {}

    // Called every frame (or editor tick)
    virtual void OnUpdate(float deltaTime) {}
    virtual void OnUiRender() {}

    //possible event handling later,,,, maybe
    //virtual void OnEvent(class Event& event) {}

    const std::string& GetName() const { return m_name; }

protected:
    Engine::Core::Renderer* m_renderer;
    std::string m_name;
};

#endif