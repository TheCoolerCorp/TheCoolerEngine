#ifndef MESH_H
#define MESH_H

#include "EngineExport.h"
#include <string>

#include "Core/GraphicsAPI/Vulkan/VulkanInterface.h"

#include "Math/vec2.h"
#include "Math/vec3.h"

#include "Vertex.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "IResource.h"

namespace Engine
{
	namespace Core
	{
		namespace RHI
		{
			class IBuffer;
		}

		class Renderer;
	}

	namespace Resource
	{
		class Mesh : public IResource
		{
		public:
			~Mesh() override = default;

			ENGINE_API void Create(std::string a_path) override;
			ENGINE_API void Destroy() override;

			ENGINE_API void Load(Core::Renderer* a_renderer) override;
			ENGINE_API void Unload(Core::Renderer* a_renderer) override;

			ENGINE_API Core::RHI::IBuffer* GetVertexBuffer() const { return m_vertexBuffer; }
			ENGINE_API Core::RHI::IBuffer* GetIndexBuffer() const { return m_indexBuffer; }
			ENGINE_API uint32_t GetNbIndices() const { return static_cast<uint32_t>(m_indexes.size()); }

		private:
			void ProcessMesh(const aiMesh* a_mesh);

			bool m_isLoaded = false;

			std::string m_path;

			std::vector<Vertex> m_vertices;
			std::vector<int> m_indexes;

			Core::RHI::IBuffer* m_vertexBuffer = nullptr;
			Core::RHI::IBuffer* m_indexBuffer = nullptr;
		};
	}
}

#endif