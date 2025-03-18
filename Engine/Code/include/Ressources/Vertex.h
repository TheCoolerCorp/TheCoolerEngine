#ifndef VERTEX_H
#define VERTEX_H

#include "Math/vec3.h"
#include "Math/vec2.h"

#include "Core/GraphicsAPI/Vulkan/VulkanUtils.h"
#include <array>

namespace Engine
{
	namespace Resource
	{
		class VulkanVertexSpec
		{
		public:
			static VkVertexInputBindingDescription getBindingDescription();
			static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions();
		};

		class Vertex
		{
		public:
			Math::vec3 mPos;
			Math::vec3 mNormals;
			Math::vec2 mUv;

			bool operator==(const Vertex& other) const
			{
				return mPos == other.mPos && mNormals == other.mNormals && mUv == other.mUv;
			}
		};
	}
}

#endif 
