#ifndef COMPONENTREGISTER_H
#define COMPONENTREGISTER_H

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>
#include <typeindex>
#include "EditorGameComponent.h"
#include "GamePlay/Components/Component.h"
#include "GamePlay/Others/GameObject.h"
namespace Editor::GamePlay
{

    using ComponentFactory = std::function<EditorGameComponent* ()>;



    /*
	* ComponentRegistry is a singleton class that manages the registration and creation of editor game components.
	* Editor Game Components that declare themselves in this class will automatically be available for use in the editor.
    */
    class ComponentRegistry {
    public:
        using ComponentAdder = std::function<void(Engine::GamePlay::GameObject&)>;

        struct Entry {
            std::string name;
            std::type_index type;
            ComponentAdder addFunction;
        };

        static ComponentRegistry& Instance() {
            static ComponentRegistry instance;
            return instance;
        }

        template<typename T>
        void Register(const std::string& name) {
            Entry t_entry{
            	name,
				std::type_index(typeid(T)),
				[](Engine::GamePlay::GameObject& obj) {
					obj.AddComponent<T>();
					}
            };
            entries.push_back(std::move(t_entry));
        }

        const std::vector<Entry>& GetEntries() const {
            return entries;
        }

    private:
        std::vector<Entry> entries;
    };

    /*
    * Helper class that automatically registers a component type with the ComponentRegistry.
    * Make a EditorGameComponent inherit from this class and pass the component type as a template parameter.
    */
    template<typename T>
    struct AutoRegisterComponent {
        static inline bool registered = [] {
            auto name = T::StaticTypeName();
            ComponentRegistry::Instance().Register<T>(name);
            return true;
            }();
    };
}
#endif // !COMPONENTREGISTER_H
