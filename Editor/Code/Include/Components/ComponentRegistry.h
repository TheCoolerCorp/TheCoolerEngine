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
	* In a nutshell, this is a mini reflection system.
    */
    class ComponentRegistry {
    public:
        using ComponentAdder = std::function<void(Engine::GamePlay::GameObject&)>;

        struct Entry {
            std::string name;
            std::type_index type;
            ComponentAdder addFunction;
            // Given a GameObject, get the component instance (as EditorGameComponent*)
            std::function<EditorGameComponent* (Engine::GamePlay::GameObject&)> getComponent;
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
                //adder function
                [](Engine::GamePlay::GameObject& obj) {
                    obj.AddComponent<T>();
                    },
                //getter function
                [](Engine::GamePlay::GameObject& obj) -> EditorGameComponent* {
                     if (T* comp = obj.GetComponent<T>()) {
                        return dynamic_cast<EditorGameComponent*>(comp);
                     }
                     return nullptr;
                }
            };
            entries.push_back(std::move(t_entry));
        }

        const std::vector<Entry>& GetEntries() const {
            return entries;
        }

        Entry* GetEntryFromId(std::type_index& a_typeIndex)
        {
			for (auto& entry : entries)
			{
				if (entry.type == a_typeIndex)
				{
					return &entry;
				}
			}
			return nullptr;
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
