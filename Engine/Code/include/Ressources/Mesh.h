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
				std::vector<Vertex> vertices = std::vector<Vertex>();
				std::vector<unsigned int> indexs = std::vector<unsigned int>();
			};

			Mesh();
			~Mesh() { delete m_data; }

			void Create(std::string& a_path, Core::RHI::ILogicalDevice* a_logicalDevice, Core::RHI::ICommandPool* a_commandPool);
			void Destroy(Core::RHI::ILogicalDevice* device);

		private:
			void ProcessMesh(const aiMesh* a_mesh);

			Data* m_data = nullptr;

		};
	}
}

#endif