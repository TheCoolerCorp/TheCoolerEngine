#include "Ressources/Mesh.h"

#include "Core/Audio/SoundManager.h"
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


        void Mesh::Load()
		{
            if (m_isLoaded.load(std::memory_order_acquire) || m_isLoading.load(std::memory_order_acquire))
            {
            	return;
            }

            m_isLoading.store(true, std::memory_order_release);

            Assimp::Importer t_importer{};
            const aiScene* t_scene = t_importer.ReadFile(m_path, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType | aiProcess_ImproveCacheLocality | aiProcess_RemoveRedundantMaterials | aiProcess_FindDegenerates | aiProcess_FindInvalidData | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph | aiProcess_GenSmoothNormals | aiProcess_FixInfacingNormals);
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

            m_isLoaded.store(true, std::memory_order_release);
            m_isLoading.store(false, std::memory_order_release);
		}

        void Mesh::BindBuffers(Core::Renderer* a_renderer)
		{
            if (m_isBound.load(std::memory_order_acquire) || m_isBinding.load(std::memory_order_acquire) || !m_isLoaded.load(std::memory_order_acquire))
            {
                return;
            }
            m_isBinding.store(true, std::memory_order_release);

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

            //m_vertices.clear();
            //m_indexes.clear();

            m_isBound.store(true, std::memory_order_release);
            m_isBinding.store(false, std::memory_order_release);

            Engine::Core::Audio::SoundManager::Get().PlaySound("Assets/Sounds/sparkle.wav");
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
            m_isBound = false;
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
