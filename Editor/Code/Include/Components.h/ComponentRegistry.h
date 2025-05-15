#ifndef COMPONENTREGISTER_H
#define COMPONENTREGISTER_H

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

namespace Editor::GamePlay
{
	class EditorGameComponent;

    using ComponentFactory = std::function<EditorGameComponent* ()>;

    /*
	* Helper class that automatically registers a component type with the ComponentRegistry.
	* Make a EditorGameComponent inherit from this class and pass the component type as a template parameter.
    */
    template<typename T>
    class AutoRegisterComponent {
    public:
        static Component* CreateInstance() { return new T(); }

    private:
		//typeid should ensure that the name is unique for each type
        static inline bool registered = [] {
            ComponentRegistry::Instance().Register(typeid(T).name(), &CreateInstance);
            return true;
            }();
    };


    /*
	* ComponentRegistry is a singleton class that manages the registration and creation of editor game components.
	* Editor Game Components that declare themselves in this class will automatically be available for use in the editor.
    */
    class ComponentRegistry {
    public:
        static ComponentRegistry& Instance() {
            static ComponentRegistry instance;
            return instance;
        }

        void Register(const std::string& typeName, ComponentFactory factory) {
            factories[typeName] = factory;
            names.push_back(typeName);
        }

        EditorGameComponent* Create(const std::string& typeName) const {
            auto it = factories.find(typeName);
            if (it != factories.end()) {
                return it->second();
            }
            return nullptr;
        }

        const std::vector<std::string>& GetAllComponentNames() const {
            return names;
        }

    private:
        std::unordered_map<std::string, ComponentFactory> factories;
        std::vector<std::string> names;
    };

}
#endif // !COMPONENTREGISTER_H
