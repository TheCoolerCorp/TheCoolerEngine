#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "EngineExport.h"

#include <vector>
#include <string>

#include "Core/Logger/Logger.h"

#include "Math/Transform.h"
#include "Component.h"
#include "Ressources/ResourceManager.h"
#include "ComponentsPool.h"


#include "Core/Interfaces/IObjectDescriptor.h"
#include "Core/Interfaces/ApiInterface.h"

#include "ServiceLocator.h"

namespace Engine
{
	namespace Core
	{
		namespace RHI
		{
			class ILogicalDevice;
		}
	}

	namespace GamePlay
	{
		struct GameObjectinfo
		{
			Core::RHI::ILogicalDevice* mLogicalDevice;
			Core::RHI::IPhysicalDevice* mPhysicalDevice;
			Core::RHI::IGraphicPipeline* mGraphicPipeline;
			Core::RHI::IDescriptorPool* mDescriptorPool;
			Core::RHI::ICommandPool* mCommandPool;
			int mSize;
		};

		struct ENGINE_API GameObjectData
		{
			Core::RHI::IBuffer* mVertexBuffer = nullptr;
			Core::RHI::IBuffer* mIndexBuffer = nullptr;
			Core::RHI::IImage* mImage = nullptr;
			Core::RHI::IObjectDescriptor* mDescriptor = nullptr;
			uint32_t mNbIndices = 0;
		};


		class GameObject
		{
		public:
			GameObject() = default;
			~GameObject() = default;

			/*GameObject(Math::vec3 a_position, Math::quat a_rotation, Math::vec3 a_scale);

			void Create(Core::RHI::ApiInterface* a_interface, GameObjectinfo a_info);
			void Update(uint32_t a_frameIndex, Engine::Core::RHI::ILogicalDevice* a_logicalDevice);
			void Destroy(Core::RHI::ApiInterface* a_interface, Core::RHI::ILogicalDevice* a_logicalDevice);
			GameObjectData SubmitData();*/



			template<typename Type, typename... Args>
			void AddComponent(Args&&... args)
			{
				/* Check if not already in c
				 * CreateComp
				 * CreateResource if needed (maybe already in resource)
				 * Add to componentsPool
				 */

				static_assert(std::is_base_of<Component, Type>::value);

				if (HasComponent<Type>())
				{
					std::string warning_error = "GameObject :" + std::to_string(m_id) + "already have this type of component" + std::to_string(typeid(Type));
					LOG_WARNING(warning_error);
					return;
				}

				std::shared_ptr<Type> t_component = std::make_shared<Type>();
				t_component->Create(args);
				//component->Create(a_path, a_interface, a_info.mPhysicalDevice, a_info.mLogicalDevice, a_info.mCommandPool);

				//m_components.emplace_back(component);
			}

			template<typename Type>
			std::shared_ptr<Type> GetComponent(ComponentsPool& a_componentsPool)
			{
				static_assert(std::is_base_of<Component, Type>::value);
				//for (const auto& component : m_components)
				//{
				//	std::shared_ptr<T> castedComponent = std::dynamic_pointer_cast<T>(component);

				//	if (castedComponent)
				//	{
				//		return castedComponent;
				//	}
				//}
				//return nullptr;
			}

			template<typename Type>
			bool HasComponent()
			{
				static_assert(std::is_base_of<Component, Type>::value);



				/*for (auto it = m_components.begin(); it != m_components.end();)
				{
					std::shared_ptr<T> component = dynamic_cast<std::shared_ptr<T>()>(it);
					if (component)
					{
						component->Destroy();
						it = m_components.erase(it);
					}
					else
					{
						++it;
					}
				}*/
			}

			std::vector<std::shared_ptr<Component>> GetComponents() { return m_components; }
			Math::Transform GetTransform() { return m_transform; }

		private:
			std::vector<std::shared_ptr<Component>> m_components;
			Math::Transform m_transform;

			Core::RHI::IObjectDescriptor* m_descriptor{};

			int m_id;
		};

		
	}
}

#endif 