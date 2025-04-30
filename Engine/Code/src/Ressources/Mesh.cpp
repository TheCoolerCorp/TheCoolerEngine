#include "Ressources/Mesh.h"

#include "Core/Renderer/Renderer.h"

namespace Engine
{
	namespace Resource
	{
		void Mesh::Create(const std::string a_path)
		{
            m_path = a_path;
		}

		void Mesh::Destroy()
		{
            m_vertices.clear();
            m_indexes.clear();
		}


        void Mesh::Load(Core::Renderer* a_renderer)
		{
            if (m_isLoaded || m_isLoading)
            {
            	return;
            }

            m_isLoading = true;

            Assimp::Importer t_importer{};
            const aiScene* t_scene = t_importer.ReadFile(m_path, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_PreTransformVertices);
            if (!t_scene || !t_scene->mRootNode)
            {
                LOG_ERROR("ERROR::ASSIMP::" + Core::Debugging::ToString(t_importer.GetErrorString()));
                return;
            }
            std::string t_directory = m_path.substr(0, m_path.find_last_of('/'));

            for (unsigned int i = 0; i < t_scene->mNumMeshes; ++i)
            {
                const aiMesh* t_mesh = t_scene->mMeshes[i];
                ProcessMesh(t_mesh);
            }

            Core::RHI::ApiInterface* t_interface = a_renderer->GetInterface();
            Core::RHI::IPhysicalDevice* t_physicalDevice = a_renderer->GetPhysicalDevice();
            Core::RHI::ILogicalDevice* t_logicalDevice = a_renderer->GetLogicalDevice();
            Core::RHI::ICommandPool* t_commandPool = a_renderer->GetCommandPool();

            m_vertexBuffer = t_interface->InstantiateBuffer();
            m_indexBuffer = t_interface->InstantiateBuffer();

            Core::RHI::BufferData t_bufferData;
            t_bufferData.mVertices = m_vertices;
            t_bufferData.mIndices = m_indexes;

            m_vertexBuffer->Create(Core::RHI::BufferType::VERTEX, t_bufferData, t_physicalDevice, t_logicalDevice, t_commandPool);
            m_indexBuffer->Create(Core::RHI::BufferType::INDEX, t_bufferData, t_physicalDevice, t_logicalDevice, t_commandPool);

            m_isLoaded = true;
            m_isLoading = false;

            //m_vertices.clear();
            //m_indexes.clear();
		}

        void Mesh::Unload(Core::Renderer* a_renderer)
		{
            if (!m_isLoaded)
            {
                return;
            }

            Core::RHI::ILogicalDevice* t_logicalDevice = a_renderer->GetLogicalDevice();

            m_vertexBuffer->Destroy(t_logicalDevice);
            m_indexBuffer->Destroy(t_logicalDevice);
            delete m_vertexBuffer;
            delete m_indexBuffer;

            m_isLoaded = false;
		}

        void Mesh::ProcessMesh(const aiMesh* mesh)
        {
            for (unsigned int i = 0; i < mesh->mNumVertices; i++)
            {
                Vertex vertex;

                if (mesh->HasPositions())
                {
                    vertex.mPos = Math::vec3(
                        mesh->mVertices[i].x,
                        mesh->mVertices[i].y,
                        mesh->mVertices[i].z
                    );
                }

                if (mesh->HasNormals())
                {
                    vertex.mNormals = Math::vec3(
                        mesh->mNormals[i].x,
                        mesh->mNormals[i].y,
                        mesh->mNormals[i].z
                    );
                }

                if (mesh->HasTextureCoords(0))
                {
                    vertex.mUv = Math::vec2(
                        mesh->mTextureCoords[0][i].x,
                        1.f - mesh->mTextureCoords[0][i].y
                    );
                }

                else
                {
                    vertex.mUv = Math::vec2(0.0f, 0.0f);
                }

                m_vertices.push_back(vertex);
            }

            for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
            {
                const aiFace face = mesh->mFaces[i];

                for (unsigned int j = 0; j < face.mNumIndices; ++j)
                {
                    m_indexes.push_back(face.mIndices[j]);
                }
            }
        }

	}
}
