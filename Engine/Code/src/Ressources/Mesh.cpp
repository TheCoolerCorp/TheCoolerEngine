#include "Ressources/Mesh.h"

namespace Engine
{
	namespace Resource
	{
		void Mesh::Create(std::string a_path, Core::RHI::ApiInterface* a_interface, Core::RHI::IPhysicalDevice* a_physicalDevice, Core::RHI::ILogicalDevice* a_logicalDevice, Core::RHI::ICommandPool* a_commandPool)
		{
            Assimp::Importer t_importer{};
            const aiScene* t_scene = t_importer.ReadFile(a_path, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_PreTransformVertices);
            if (!t_scene || !t_scene->mRootNode)
            {
                std::cout << "ERROR::ASSIMP::" << t_importer.GetErrorString() << '\n';
                return;
            }
            std::string t_directory = a_path.substr(0, a_path.find_last_of('/'));

            for (unsigned int i = 0; i < t_scene->mNumMeshes; ++i)
            {
                const aiMesh* t_mesh = t_scene->mMeshes[i];
                ProcessMesh(t_mesh);
            }
            m_vertexBuffer = a_interface->InstantiateBuffer();
            m_indexBuffer = a_interface->InstantiateBuffer();

            Core::RHI::BufferData t_bufferData;
            t_bufferData.mVertices = m_vertices;
            t_bufferData.mIndices = m_indexes;

            m_vertexBuffer->Create(Core::RHI::BufferType::VERTEX, t_bufferData, a_physicalDevice, a_logicalDevice, a_commandPool);
            m_indexBuffer->Create(Core::RHI::BufferType::INDEX, t_bufferData, a_physicalDevice, a_logicalDevice, a_commandPool);
		}
		void Mesh::Destroy(Core::RHI::ILogicalDevice* a_logicalDevice)
		{
            m_vertices.clear();
            m_indexes.clear();
            m_vertexBuffer->Destroy(a_logicalDevice);
            m_indexBuffer->Destroy(a_logicalDevice);
            delete m_vertexBuffer;
            delete m_indexBuffer;
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
                        mesh->mTextureCoords[0][i].y
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