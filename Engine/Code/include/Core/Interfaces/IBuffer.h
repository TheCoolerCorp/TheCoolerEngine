#ifndef IBUFFER_H
#define IBUFFER_H

#include <vector>

#include "EngineExport.h"
#include "Core/Logger/Logger.h"
#include "Ressources/Vertex.h"

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			class VulkanBuffer;
		}

		namespace RHI
		{
			class ILogicalDevice;
			class ICommandPool;
			class IPhysicalDevice;

			enum class BufferType : int
			{
				NONE = 0,
				VERTEX = 1,
				INDEX = 2,
				UBO = 3
			};

			struct BufferData
			{
				std::vector<Resource::Vertex> mVertices{};
				std::vector<int> mIndices{};
				void* mUboData = nullptr;
				size_t mUboSize = 0;
			};

			class IBuffer
			{
			public:
				ENGINE_API virtual ~IBuffer() = default;
				ENGINE_API virtual void Create(BufferType a_type, BufferData a_data, IPhysicalDevice* a_physicalDevice, ILogicalDevice* a_logicalDevice, ICommandPool* a_commandPool) = 0;
				ENGINE_API virtual void Destroy(ILogicalDevice* a_logicalDevice) = 0;
				ENGINE_API virtual GraphicsAPI::VulkanBuffer* CastVulkan() { LOG_ERROR("Try to return wrong cast type : VulkanCommandPool!"); return nullptr; }
			};
		}
	}
}

#endif