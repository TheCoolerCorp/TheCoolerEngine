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


namespace Engine
{
	namespace Ressources
	{

		class ENGINE_API Mesh
		{
		public:
			struct Data
			{
				std::vector<Vertex> mVertices = std::vector<Vertex>();
				std::vector<unsigned int> mIndexs = std::vector<unsigned int>();
				int mSize = 0;
			};

			Mesh();
			~Mesh() { delete m_data; }

			void Create(std::string a_path);
			void Destroy();

			Data* GetData() { return m_data; }

			void ProcessMesh(const aiMesh* a_mesh);

			Data* m_data = nullptr;
		private:

		};
	}
}

#endif