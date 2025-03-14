#include "Ressources/Mesh.h"

namespace Engine
{
	namespace Ressources
	{
        Mesh::Mesh() : m_data(new Data) {}


		void Mesh::Create(std::string& a_path, Core::RHI::ILogicalDevice* a_logicalDevice, Core::RHI::ICommandPool* a_commandPool)
		{
			
		}
		void Mesh::Destroy(Core::RHI::ILogicalDevice* device)
		{
			
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

                m_data->vertices.push_back(vertex);
            }

            for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
            {
                const aiFace face = mesh->mFaces[i];

                for (unsigned int j = 0; j < face.mNumIndices; ++j)
                {
                    m_data->indexs.push_back(face.mIndices[j]);
                }
            }
        }

	}
}