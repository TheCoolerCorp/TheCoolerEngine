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

			class ENGINE_API IBuffer
			{
			public:
				virtual ~IBuffer() = default;
				virtual void CreateVertexBuffer(std::vector<Ressources::Vertex> a_vertices) = 0;
				virtual void CreateIndexBuffer(std::vector<uint32_t> a_indices) = 0;
				virtual void Destroy(ILogicalDevice* a_logicalDevice) = 0;
				virtual GraphicsAPI::VulkanBuffer* CastVulkan() { LOG_ERROR("Try to return wrong cast type : VulkanCommandPool!"); return nullptr; }
			};
		}
	}
}

#endif