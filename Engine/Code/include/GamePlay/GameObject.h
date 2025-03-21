#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "EngineExport.h"

#include <vector>
#include <string>

#include "Core/Logger/Logger.h"

#include "Math/Transform.h"
#include "Component.h"
#include "Core/Interfaces/IObjectDescriptor.h"
#include "Core/Interfaces/ApiInterface.h"

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

			GameObject(Math::vec3 a_position, Math::quat a_rotation, Math::vec3 a_scale);

			void Create(Core::RHI::ApiInterface* a_interface, GameObjectinfo a_info);
			void Update(uint32_t a_frameIndex, Engine::Core::RHI::ILogicalDevice* a_logicalDevice);
			void Destroy(Core::RHI::ApiInterface* a_interface, Core::RHI::ILogicalDevice* a_logicalDevice);
			GameObjectData SubmitData();
			

			template<typename T>
			void AddComponent(std::string a_path, GameObjectinfo a_info, Core::RHI::ApiInterface* a_interface)
			{
				static_assert(std::is_base_of<Component, T>::value, "Type is not a component");

				if (GetComponent<T>())
				{
					LOG_ERROR("Already as this type of component");
					return;
				}

				std::shared_ptr<T> component = std::make_shared<T>();

				component->Create(a_path, a_interface, a_info.mPhysicalDevice, a_info.mLogicalDevice, a_info.mCommandPool);

				m_components.emplace_back(component);
			}

			template<typename T>
			std::shared_ptr<T> GetComponent()
			{
				static_assert(std::is_base_of<Component, T>::value, "Type is not a component");

				for (const auto& component : m_components)
				{
					std::shared_ptr<T> castedComponent = std::dynamic_pointer_cast<T>(component);

					if (castedComponent)
					{
						return castedComponent;
					}
				}
				return nullptr;
			}

			template<typename T>
			void RemoveComponent()
			{
				static_assert(std::is_base_of<Component, T>::value, "Type is not a component");

				for (auto it = m_components.begin(); it != m_components.end();)
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
				}
			}

			std::vector<std::shared_ptr<Component>> GetComponents() { return m_components; }
			Math::Transform GetTransform() { return m_transform; }

		private:
			std::vector<std::shared_ptr<Component>> m_components;
			Math::Transform m_transform;

			Core::RHI::IObjectDescriptor* m_descriptor{};
		};

		
	}
}

#endif 