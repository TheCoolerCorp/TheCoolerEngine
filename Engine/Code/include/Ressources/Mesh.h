#ifndef MESH_H
#define MESH_H

#include "EngineExport.h"
#include <string>

#include "Core/GraphicsAPI/Vulkan/VulkanLogicalDevice.h"
#include "Core/GraphicsAPI/Vulkan/VulkanCommandPool.h"

#include "Math/vec2.h"
#include "Math/vec3.h"

#include "Vertex.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "IResource.h"
namespace Engine
{
	namespace Resource
	{
		class Mesh : public IResource
		{
		public:
			ENGINE_API void Create(std::string a_path) override;
			ENGINE_API void Destroy() override;

		private:
			std::vector<Vertex> m_vertices;
			std::vector<int> m_indexes;

			void ProcessMesh(const aiMesh* a_mesh);
		};
	}
}

#endif