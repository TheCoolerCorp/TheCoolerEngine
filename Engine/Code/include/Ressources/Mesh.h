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

			ENGINE_API void Load() override;
			ENGINE_API void BindBuffers(Core::Renderer* a_renderer);
			ENGINE_API void Unload(Core::Renderer* a_renderer) override;

			ENGINE_API Core::RHI::IBuffer* GetVertexBuffer() const { return m_vertexBuffer; }
			ENGINE_API Core::RHI::IBuffer* GetIndexBuffer() const { return m_indexBuffer; }

			[[nodiscard]] ENGINE_API bool IsBound() const { return m_isBound.load(std::memory_order_acquire); }

			[[nodiscard]] ENGINE_API uint32_t GetNbIndices() const { return static_cast<uint32_t>(m_indexes.size()); }
			[[nodiscard]] ENGINE_API uint32_t GetNbVertices() const { return static_cast<uint32_t>(m_vertices.size()); }
			[[nodiscard]] ENGINE_API const std::string& GetPath() const { return m_path; }

		private:
			void ProcessMesh(const aiMesh* a_mesh);

			std::string m_path;

			std::atomic<bool> m_isBound = false;
			std::atomic<bool> m_isBinding = false;

			std::vector<Vertex> m_vertices;
			std::vector<int> m_indexes;

			Core::RHI::IBuffer* m_vertexBuffer = nullptr;
			Core::RHI::IBuffer* m_indexBuffer = nullptr;
		};
	}
}

#endif