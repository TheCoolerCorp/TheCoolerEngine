#include "GamePlay/Systems/RenderSystem.h"

#include "Math/Transform.h"
#include "GamePlay/ServiceLocator.h"

//#include "GamePlay/Others/Camera.h"
#include "Core/Window/IInputHandler.h"
namespace Engine
{
	namespace GamePlay
	{
		void RenderSystem::Create(Core::Renderer* a_renderer)
		{
			m_renderer = a_renderer;
			m_lightsDescriptor = a_renderer->GetInterface()->InstantiateObjectDescriptor();
			m_lightsDescriptor->Create(a_renderer->GetLogicalDevice(), a_renderer->GetLitPipeline(), Core::RHI::Lights, 1, 1, { 1 }, { Core::RHI::DescriptorSetDataType::DESCRIPTOR_SET_TYPE_UNIFORM_BUFFER });
			uint32_t t_lightsSize = MAX_LIGHTS * sizeof(LightData);
			for (int i = 0; i < MAX_LIGHTS; ++i)
			{
				m_lightsData[i] = { Math::vec3(0.f), Math::vec3(0.f), 0.f };
			}
			m_lightsDescriptor->SetUniform(a_renderer->GetLogicalDevice(), a_renderer->GetPhysicalDevice(), a_renderer->GetCommandPool(), 0, &m_lightsData, t_lightsSize, 0, 1);
		}

		void RenderSystem::Update(Core::Renderer* a_renderer, std::vector<std::pair<int,Math::UniformMatrixs>> a_updatedMatrix, std::vector<std::pair<int, Math::vec3>> a_lightsUpdate, std::vector<int> a_materialUpdate, std::vector<std::pair<int, Math::mat4>> a_cameraUpdatedMatrix)
		{
			if (a_updatedMatrix.empty())
			{
				return;
			}
			if (!m_renderer)
				m_renderer = a_renderer;

			Core::RHI::ILogicalDevice* t_logicalDevice = a_renderer->GetLogicalDevice();
			Core::RHI::IPhysicalDevice* t_physicalDevice = a_renderer->GetPhysicalDevice();
			Core::RHI::ISurface* t_surface = a_renderer->GetSurface();
			Core::RHI::ICommandPool* t_commandPool = a_renderer->GetCommandPool();
			Core::RHI::IGraphicPipeline* t_unlitPipeline = a_renderer->GetUnlitPipeline();
			Core::RHI::IGraphicPipeline* t_litPipeline = a_renderer->GetLitPipeline();
			Core::RHI::IGraphicPipeline* t_skyBoxPipeline = a_renderer->GetSkyBoxPipeline();
			int t_maxFrame = a_renderer->GetSwapChain()->GetMaxFrame();

			CreatePendingComponentsDescriptors(a_renderer, t_logicalDevice, t_physicalDevice, t_surface, t_commandPool, t_unlitPipeline, t_litPipeline, t_skyBoxPipeline, t_maxFrame, a_updatedMatrix);
			CreatePendingLightComponentsDescriptors(a_renderer->GetInterface(), t_logicalDevice, t_physicalDevice, t_surface, t_commandPool, t_litPipeline, 1);

			for (int i = 0; i < a_updatedMatrix.size(); ++i)
			{
				if (a_updatedMatrix[i].first == -1)
					continue;
				if (m_objectsDescriptors[a_updatedMatrix[i].first]->GetPipelineTargetType() == SKYBOX)
					continue;
				m_objectsDescriptors[a_updatedMatrix[i].first]->UpdateUniforms(t_logicalDevice, 0, &a_updatedMatrix[i].second, sizeof(Math::UniformMatrixs), a_renderer->GetSwapChain()->GetCurrentFrame());
			}
			for (int i = 0; i < a_lightsUpdate.size(); ++i)
			{
				/*
				 *
				 * ISSUE HERE
				 */
				m_lightComponents[a_lightsUpdate[i].first]->GetLight().SetPosition(a_lightsUpdate[i].second);
				m_lightsData[a_lightsUpdate[i].first].m_position = a_lightsUpdate[i].second;
				m_lightsData[a_lightsUpdate[i].first].m_color = m_lightComponents[a_lightsUpdate[i].first]->GetLight().GetColor();
				m_lightsData[a_lightsUpdate[i].first].m_intensity = m_lightComponents[a_lightsUpdate[i].first]->GetLight().GetIntensisty();
			}
			m_lightsDescriptor->UpdateUniforms(t_logicalDevice, 0, &m_lightsData, MAX_LIGHTS * sizeof(LightData), 0);

			for (int i = 0; i < a_cameraUpdatedMatrix.size(); ++i)
			{
				if (a_cameraUpdatedMatrix[i].first == -1)
					continue;
				//m_cameraComponents[a_updatedMatrix[i].first]->UpdateUniforms(t_logicalDevice, 0, &a_updatedMatrix[i].second, sizeof(Math::UniformMatrixs), a_renderer->GetSwapChain()->GetCurrentFrame());
			}

			UpdateMaterial(a_renderer, t_logicalDevice, t_physicalDevice, t_surface, t_commandPool, t_unlitPipeline, t_litPipeline, t_maxFrame, a_materialUpdate, a_updatedMatrix);
		}

		void RenderSystem::UpdateCamera(Core::Renderer* a_renderer, float a_deltaTime, Core::Window::IWindow* a_window, Core::Window::IInputHandler* a_inputHandler, int a_cameraIndex, Math::mat4 a_model)
		{
			if (a_cameraIndex < m_cameraComponents.size())
			{
				m_cameraComponents[a_cameraIndex]->GetCamera().Update(m_renderer, a_inputHandler, a_window, a_deltaTime, a_model);
			}
		}

		void RenderSystem::Destroy(Core::Renderer* a_renderer)
		{
			// Destroy Meshes
			for (int i = 0; i < m_components.size(); ++i)
			{
				if (auto& comp = m_components[i])
				{
					comp->Destroy();
					delete comp;
				}
				if (m_objectsDescriptors[i])
				{
					m_objectsDescriptors[i]->Destroy(a_renderer->GetLogicalDevice());
					a_renderer->GetInterface()->DestroyObjectDescriptor(m_objectsDescriptors[i]);
				}
			}
			m_components.clear();
			m_availableIndexes.clear();
			m_pendingComponents.clear();

			// Destroy lights
			for (int i = 0; i < m_lightComponents.size(); ++i)
			{
				if (auto& comp = m_lightComponents[i])
				{
					if (comp)
					{
						comp->Destroy();
						delete comp;
						
					}
				}
			}
			if (m_lightsDescriptor)
			{
				m_lightsDescriptor->Destroy(a_renderer->GetLogicalDevice());
				a_renderer->GetInterface()->DestroyObjectDescriptor(m_lightsDescriptor);
			}
			m_lightComponents.clear();
			m_lightsAvailableIndexes.clear();
			m_lightsPendingComponents.clear();

			for (int i = 0; i < m_cameraComponents.size(); ++i)
			{
				if (m_cameraComponents[i])
				{
					m_cameraComponents[i]->Destroy();
					m_cameraComponents[i]->GetCamera().Destroy(a_renderer);
					delete m_cameraComponents[i];
				}

			}
			m_cameraComponents.clear();
			m_cameraAvailableIndexes.clear();
			m_cameraPendingComponents.clear();
		}


		int RenderSystem::AddComponent(MeshComponent* a_meshComponent)
		{
			if (m_availableIndexes.empty())
			{
				m_components.emplace_back(a_meshComponent);
				const int t_nbComps = static_cast<int>(m_components.size() - 1);
				m_pendingComponents.push_back(t_nbComps);
				return t_nbComps;
			}
			int t_index = m_availableIndexes.back();
			m_availableIndexes.pop_back();
			m_components[t_index] = a_meshComponent;
			m_pendingComponents.push_back(t_index);

			return t_index;
			// ADD PENDING INDEX
		}

		int RenderSystem::AddComponent(LightComponent* a_lightComponent)
		{
			if (m_lightsAvailableIndexes.empty())
			{
				m_lightComponents.emplace_back(a_lightComponent);
				const int t_nbComps = static_cast<int>(m_lightComponents.size() - 1);
				m_lightsPendingComponents.push_back(t_nbComps);
				return t_nbComps;
			}
			for (const int t_availableIndex : m_lightsAvailableIndexes)
			{
				if (m_lightComponents.at(t_availableIndex) == nullptr)
				{
					m_lightComponents.at(t_availableIndex) = a_lightComponent;
					m_lightsPendingComponents.push_back(t_availableIndex);
					return t_availableIndex;
				}
			}
			return -1;
			// ADD PENDING INDEX
		}

		int RenderSystem::AddComponent(CameraComponent* a_cameraComponent)
		{
			if (m_cameraAvailableIndexes.empty())
			{
				m_cameraComponents.emplace_back(a_cameraComponent);
				const int t_nbComps = static_cast<int>(m_cameraComponents.size() - 1);
				m_cameraPendingComponents.push_back(t_nbComps);
				return t_nbComps;
			}
			for (const int t_availableIndex : m_cameraAvailableIndexes)
			{
				if (m_lightComponents.at(t_availableIndex) == nullptr)
				{
					m_cameraComponents.at(t_availableIndex) = a_cameraComponent;
					m_cameraPendingComponents.push_back(t_availableIndex);
					return t_availableIndex;
				}
			}
			return -1;
		}

		void RenderSystem::RemoveMeshComponent(const int a_id)
		{
			if (m_components.at(a_id) != nullptr && a_id < m_components.size())
			{
				m_renderer->GetLogicalDevice()->WaitIdle();
				if (m_components.at(a_id)->GetMesh() != nullptr)
				{
					//m_components.at(a_id)->GetMesh()->Unload(m_renderer);
				}
				m_components.at(a_id)->Destroy();
				delete m_components.at(a_id);
				m_components[a_id] = nullptr;
				m_availableIndexes.push_back(a_id);

				m_objectsDescriptors[a_id]->Destroy(m_renderer->GetLogicalDevice());
				delete m_objectsDescriptors[a_id];
				m_objectsDescriptors[a_id] = nullptr;
			}
		}

		void RenderSystem::RemoveLightComponent(const int a_id)
		{
			if (m_lightComponents.at(a_id) != nullptr && a_id < m_lightComponents.size())
			{
				if (m_lightComponents.at(a_id))
				{
					m_lightComponents.at(a_id)->Destroy();
					delete m_lightComponents.at(a_id);
					m_lightComponents[a_id] = nullptr;
					m_availableIndexes.push_back(a_id);

					m_lightsData[a_id].m_position = Math::vec3(0.f);
					m_lightsData[a_id].m_color = Math::vec3(0.f);
					m_lightsData[a_id].m_intensity = 0.f;
				}
			}
		}

		void RenderSystem::RemoveCameraComponent(int a_id)
		{
			if (m_cameraComponents.at(a_id) != nullptr && a_id < m_cameraComponents.size())
			{
				if (m_cameraComponents.at(a_id))
				{
					m_cameraComponents.at(a_id)->GetCamera().Destroy(m_renderer);
					m_cameraComponents.at(a_id)->Destroy();
					delete m_cameraComponents.at(a_id);
					m_cameraComponents[a_id] = nullptr;
					m_cameraAvailableIndexes.push_back(a_id);
				}
			}
		}

		MeshComponent* RenderSystem::GetMeshComponent(const int a_id) const
		{
			if (a_id >= static_cast<int>(m_components.size()))
			{
				return nullptr;
			}
			return m_components.at(a_id);
		}

		Core::RHI::IObjectDescriptor* RenderSystem::GetMeshDescriptor(int a_idx)
		{
			if (a_idx >= static_cast<int>(m_objectsDescriptors.size()) || a_idx < 0)
			{
				return nullptr;
			}
			return m_objectsDescriptors[a_idx];
		}

		LightComponent* RenderSystem::GetLightComponent(const int a_id) const
		{
			if (a_id >= static_cast<int>(m_lightComponents.size()))
			{
				return nullptr;
			}
			return m_lightComponents.at(a_id);
		}

		CameraComponent* RenderSystem::GetCameraComponent(int a_id) const
		{
			if (a_id >= static_cast<int>(m_cameraComponents.size()))
			{
				return nullptr;
			}
			return m_cameraComponents.at(a_id);
		}

		Core::RHI::IObjectDescriptor* RenderSystem::GetCameraDescriptor(int a_id)
		{
			if (a_id >= static_cast<int>(m_cameraComponents.size()))
			{
				return nullptr;
			}
			return m_cameraComponents.at(a_id)->GetCamera().GetDescriptor();
		}

		void RenderSystem::CreatePendingComponentsDescriptors(Core::Renderer* a_renderer, Core::RHI::ILogicalDevice* a_logicalDevice,
			Core::RHI::IPhysicalDevice* a_physicalDevice, Core::RHI::ISurface* a_surface, Core::RHI::ICommandPool* a_commandPool,
			Core::RHI::IGraphicPipeline* a_unlitPipeline, Core::RHI::IGraphicPipeline* a_litPipeine, Core::RHI::IGraphicPipeline* a_skyBoxPipeline,uint32_t a_maxFrame, std::vector<std::pair<int, Math::UniformMatrixs>>& a_updatedMatrix)
		{
			a_logicalDevice->WaitIdle();

			Core::RHI::ApiInterface* apiInterface = a_renderer->GetInterface();

			std::vector<int> t_componentsToErase;

			for (int i = 0; i < m_pendingComponents.size(); ++i)
			{
				if (!m_components[m_pendingComponents[i]]->GetMesh()->IsLoaded())
				{
					continue;
				}

				Core::RHI::IObjectDescriptor* t_newRenderObject = apiInterface->InstantiateObjectDescriptor();

				Ref<Material> t_material = m_components[m_pendingComponents[i]]->GetMaterial();

				std::vector<Core::RHI::DescriptorSetDataType> m_types;
				if (t_material->GetType() == UNLIT)
				{
					m_types = {Core::RHI::DescriptorSetDataType::DESCRIPTOR_SET_TYPE_UNIFORM_BUFFER , Core::RHI::DescriptorSetDataType::DESCRIPTOR_SET_TYPE_COMBINED_IMAGE_SAMPLER};
					t_newRenderObject->Create(a_logicalDevice, a_unlitPipeline, Core::RHI::Object, 3, 1, { a_maxFrame }, m_types);
				}
				else if (t_material->GetType() == SKYBOX)
				{
					m_types = {Core::RHI::DescriptorSetDataType::DESCRIPTOR_SET_TYPE_COMBINED_IMAGE_SAMPLER };
					t_newRenderObject->Create(a_logicalDevice, a_skyBoxPipeline, Core::RHI::Object, 1, 1, { 1 }, m_types);
				}
				else if (t_material->GetType() == LIT)
				{
					m_types = { Core::RHI::DescriptorSetDataType::DESCRIPTOR_SET_TYPE_UNIFORM_BUFFER, Core::RHI::DescriptorSetDataType::DESCRIPTOR_SET_TYPE_COMBINED_IMAGE_SAMPLER,
								Core::RHI::DescriptorSetDataType::DESCRIPTOR_SET_TYPE_COMBINED_IMAGE_SAMPLER, Core::RHI::DescriptorSetDataType::DESCRIPTOR_SET_TYPE_COMBINED_IMAGE_SAMPLER,
								Core::RHI::DescriptorSetDataType::DESCRIPTOR_SET_TYPE_COMBINED_IMAGE_SAMPLER, Core::RHI::DescriptorSetDataType::DESCRIPTOR_SET_TYPE_COMBINED_IMAGE_SAMPLER,
								Core::RHI::DescriptorSetDataType::DESCRIPTOR_SET_TYPE_UNIFORM_BUFFER, Core::RHI::DescriptorSetDataType::DESCRIPTOR_SET_TYPE_UNIFORM_BUFFER};

					t_newRenderObject->Create(a_logicalDevice, a_litPipeine, Core::RHI::Object, 3,3, { a_maxFrame, 1, 1 }, m_types);

					if (t_material->HasNormal())
					{
						Ref<Resource::Texture> t_normal = t_material->GetNormal();
						if (!t_normal->IsCreated())
						{
							const Ref<Resource::Texture> t_defaultTexture = ServiceLocator::GetResourceManager()->GetResource<Resource::Texture>("Assets/Textures/DefaultTexture.png");
							t_normal->CreateImage(a_renderer);
							t_newRenderObject->SetTexture(a_logicalDevice, t_defaultTexture->GetImage(), 2, 1);
						}
						else
						{
							t_newRenderObject->SetTexture(a_logicalDevice, t_normal->GetImage(), 2, 1);
						}
					}
					else
					{
						t_newRenderObject->SetTexture(a_logicalDevice, t_material->GetEmpty()->GetImage(), 2, 1);
					}
					if (t_material->HasMetallic())
					{
						Ref<Resource::Texture> t_metallic = t_material->GetMetallic();
						if (!t_metallic->IsCreated())
						{
							const Ref<Resource::Texture> t_defaultTexture = ServiceLocator::GetResourceManager()->GetResource<Resource::Texture>("Assets/Textures/DefaultTexture.png");
							t_metallic->CreateImage(a_renderer);
							t_newRenderObject->SetTexture(a_logicalDevice, t_defaultTexture->GetImage(), 3, 1);
						}
						else
						{
							t_newRenderObject->SetTexture(a_logicalDevice, t_metallic->GetImage(), 3, 1);
						}
					}
					else
					{
						t_newRenderObject->SetTexture(a_logicalDevice, t_material->GetEmpty()->GetImage(), 3, 1);
					}
					if (t_material->HasRoughness())
					{
						Ref<Resource::Texture> t_roughness = t_material->GetRoughness();
						if (!t_roughness->IsCreated())
						{
							const Ref<Resource::Texture> t_defaultTexture = ServiceLocator::GetResourceManager()->GetResource<Resource::Texture>("Assets/Textures/DefaultTexture.png");
							t_roughness->CreateImage(a_renderer);
							t_newRenderObject->SetTexture(a_logicalDevice, t_defaultTexture->GetImage(), 3, 1);
						}
						else
						{
							t_newRenderObject->SetTexture(a_logicalDevice, t_roughness->GetImage(), 3, 1);
						}
					}
					else
					{
						t_newRenderObject->SetTexture(a_logicalDevice, t_material->GetEmpty()->GetImage(), 4, 1);
					}
					if (t_material->HasAO())
					{
						Ref<Resource::Texture> t_ao = t_material->GetAO();
						if (!t_ao->IsCreated())
						{
							const Ref<Resource::Texture> t_defaultTexture = ServiceLocator::GetResourceManager()->GetResource<Resource::Texture>("Assets/Textures/DefaultTexture.png");
							t_ao->CreateImage(a_renderer);
							t_newRenderObject->SetTexture(a_logicalDevice, t_defaultTexture->GetImage(), 3, 1);
						}
						else
						{
							t_newRenderObject->SetTexture(a_logicalDevice, t_ao->GetImage(), 3, 1);
						}
					}
					else
					{
						t_newRenderObject->SetTexture(a_logicalDevice, t_material->GetEmpty()->GetImage(), 5, 1);
					}
					t_newRenderObject->SetUniform(a_logicalDevice, a_physicalDevice, a_commandPool, 1, &t_material->GetMaterialValues(), sizeof(MaterialValues), 6, 1);
					t_newRenderObject->SetUniform(a_logicalDevice, a_physicalDevice, a_commandPool, 2, &t_material->GetHasTextures(), sizeof(HasMaterialTextures), 7, 1);
				}
				else
				{
					LOG_ERROR("Not other type of pipeline has been implemented");
				}
				if (t_material->GetType() != SKYBOX)
				{
					t_newRenderObject->SetUniform(a_logicalDevice, a_physicalDevice, a_commandPool, 0, &a_updatedMatrix.at(m_pendingComponents.at(i)).second.m_transform, sizeof(Math::UniformMatrixs), 0, 1);
				}

				Ref<Resource::Texture> t_albedo = t_material->GetAlbedo();
				if (t_albedo)
				{
					if (t_albedo->IsCreated())
					{
 						t_newRenderObject->SetTexture(a_logicalDevice, t_albedo->GetImage(), 1, 1);

					}
					else
					{
						Resource::ResourceManager* t_resourceManager = ServiceLocator::GetResourceManager();
						const Ref<Resource::Texture> t_defaultTexture = t_material->GetType() == SKYBOX ? t_resourceManager->GetResource<Resource::Texture>("Assets/Textures/CubeMap/default_cubemap.png") : t_resourceManager->GetResource<Resource::Texture>("Assets/Textures/DefaultTexture.png");
						t_newRenderObject->SetTexture(a_logicalDevice, t_defaultTexture->GetImage(), 1, 1);
						t_albedo->CreateImage(a_renderer);
					}
				}
				else
				{
					Resource::ResourceManager* t_resourceManager = ServiceLocator::GetResourceManager();
					const Ref<Resource::Texture> t_defaultTexture = t_material->GetType() == SKYBOX ? t_resourceManager->GetResource<Resource::Texture>("Assets/Textures/CubeMap/default_cubemap.png") : t_resourceManager->GetResource<Resource::Texture>("Assets/Textures/DefaultTexture.png");
					t_material->SetAlbedo(t_defaultTexture);
					t_newRenderObject->SetTexture(a_logicalDevice, t_defaultTexture->GetImage(), 1, 1);
				}

				
				if (m_pendingComponents[i] >= static_cast<int>(m_objectsDescriptors.size()))
				{
					m_objectsDescriptors.push_back(t_newRenderObject);
				}
				else
				{
					m_objectsDescriptors[m_pendingComponents[i]] = t_newRenderObject;
				}

				t_componentsToErase.push_back(m_pendingComponents[i]);
			}

			for (int i : t_componentsToErase)
			{
				m_pendingComponents.erase(std::ranges::remove(m_pendingComponents, i).begin(), m_pendingComponents.end());

			}
			t_componentsToErase.clear();
		}

		void RenderSystem::CreatePendingLightComponentsDescriptors(Core::RHI::ApiInterface* apiInterface, Core::RHI::ILogicalDevice* a_logicalDevice,
			Core::RHI::IPhysicalDevice* a_physicalDevice, Core::RHI::ISurface* a_surface, Core::RHI::ICommandPool* a_commandPool,
			Core::RHI::IGraphicPipeline* a_litPipeine, uint32_t a_count)
		{
			for (int i = 0; i < m_lightsPendingComponents.size() && m_lightComponents.size() <= MAX_LIGHTS; ++i)
			{
				m_lightsData[i] = m_lightComponents[m_lightsPendingComponents[i]]->GetLight().GetData();
			}
			m_lightsPendingComponents.clear();
		}

		void RenderSystem::CreatePendingCameraComponentsDescriptors(Core::RHI::ApiInterface* apiInterface, Core::RHI::ILogicalDevice* a_logicalDevice,
			Core::RHI::IPhysicalDevice* a_physicalDevice, Core::RHI::ISurface* a_surface, Core::RHI::ICommandPool* a_commandPool,
			Core::RHI::IGraphicPipeline* a_litPipeine, uint32_t a_count)
		{
			for (int i = 0; i < m_cameraPendingComponents.size(); ++i)
			{
				m_cameraComponents[m_cameraPendingComponents[i]]->GetCamera().Create(m_renderer);
			}
			m_cameraPendingComponents.clear();
		}

		void RenderSystem::UpdateMaterial(Core::Renderer* a_renderer, Core::RHI::ILogicalDevice* a_logicalDevice,
			Core::RHI::IPhysicalDevice* a_physicalDevice, Core::RHI::ISurface* a_surface, Core::RHI::ICommandPool* a_commandPool, Core::RHI::IGraphicPipeline* a_unlitPipeine,
			Core::RHI::IGraphicPipeline* a_litPipeine, uint32_t a_maxFrame, std::vector<int>& a_indexes, std::vector<std::pair<int, Math::UniformMatrixs>>& a_updatedMatrix)
		{

			a_logicalDevice->WaitIdle();
			Core::RHI::ApiInterface* apiInterface = a_renderer->GetInterface();

			for (int i = 0; i < a_indexes.size(); ++i)
			{
				Core::RHI::IObjectDescriptor* t_newRenderObject = apiInterface->InstantiateObjectDescriptor();

				Ref<Material> t_material = m_components[a_indexes[i]]->GetMaterial();
				t_material->SetNeedUpdate(false);

				std::vector<Core::RHI::DescriptorSetDataType> m_types;
				if (t_material->GetType() == UNLIT)
				{
					m_types = { Core::RHI::DescriptorSetDataType::DESCRIPTOR_SET_TYPE_UNIFORM_BUFFER , Core::RHI::DescriptorSetDataType::DESCRIPTOR_SET_TYPE_COMBINED_IMAGE_SAMPLER };
					t_newRenderObject->Create(a_logicalDevice, a_unlitPipeine, Core::RHI::Object, 3, 1, { a_maxFrame }, m_types);
				}
				else if (t_material->GetType() == LIT)
				{
					m_types = { Core::RHI::DescriptorSetDataType::DESCRIPTOR_SET_TYPE_UNIFORM_BUFFER, Core::RHI::DescriptorSetDataType::DESCRIPTOR_SET_TYPE_COMBINED_IMAGE_SAMPLER,
								Core::RHI::DescriptorSetDataType::DESCRIPTOR_SET_TYPE_COMBINED_IMAGE_SAMPLER, Core::RHI::DescriptorSetDataType::DESCRIPTOR_SET_TYPE_COMBINED_IMAGE_SAMPLER,
								Core::RHI::DescriptorSetDataType::DESCRIPTOR_SET_TYPE_COMBINED_IMAGE_SAMPLER, Core::RHI::DescriptorSetDataType::DESCRIPTOR_SET_TYPE_COMBINED_IMAGE_SAMPLER,
								Core::RHI::DescriptorSetDataType::DESCRIPTOR_SET_TYPE_UNIFORM_BUFFER, Core::RHI::DescriptorSetDataType::DESCRIPTOR_SET_TYPE_UNIFORM_BUFFER };

					t_newRenderObject->Create(a_logicalDevice, a_litPipeine, Core::RHI::Object, 3, 3, { a_maxFrame, 1, 1 }, m_types);

					if (t_material->HasNormal())
					{
						Ref<Resource::Texture> t_normal = t_material->GetNormal();
						if (!t_normal->IsCreated())
						{
							t_normal->CreateImage(a_renderer);
							const Ref<Resource::Texture> t_defaultTexture = ServiceLocator::GetResourceManager()->GetResource<Resource::Texture>("Assets/Textures/DefaultTexture.png");
							t_newRenderObject->SetTexture(a_logicalDevice, t_defaultTexture->GetImage(), 2, 1);
							t_material->SetNeedUpdate(true);
						}
						else
						{
							t_newRenderObject->SetTexture(a_logicalDevice, t_normal->GetImage(), 2, 1);
						}
					}
					else
					{
						t_newRenderObject->SetTexture(a_logicalDevice, t_material->GetEmpty()->GetImage(), 2, 1);
					}
					if (t_material->HasMetallic())
					{
						Ref<Resource::Texture> t_metallic = t_material->GetMetallic();
						if (!t_metallic->IsCreated())
						{
							t_metallic->CreateImage(a_renderer);
							const Ref<Resource::Texture> t_defaultTexture = ServiceLocator::GetResourceManager()->GetResource<Resource::Texture>("Assets/Textures/DefaultTexture.png");
							t_newRenderObject->SetTexture(a_logicalDevice, t_defaultTexture->GetImage(), 3, 1);
							t_material->SetNeedUpdate(true);
						}
						else
						{
							t_newRenderObject->SetTexture(a_logicalDevice, t_metallic->GetImage(), 3, 1);
						}
					}
					else
					{
						t_newRenderObject->SetTexture(a_logicalDevice, t_material->GetEmpty()->GetImage(), 3, 1);
					}
					if (t_material->HasRoughness())
					{
						Ref<Resource::Texture> t_roughness = t_material->GetRoughness();
						if (!t_roughness->IsCreated())
						{
							t_roughness->CreateImage(a_renderer);
							const Ref<Resource::Texture> t_defaultTexture = ServiceLocator::GetResourceManager()->GetResource<Resource::Texture>("Assets/Textures/DefaultTexture.png");
							t_newRenderObject->SetTexture(a_logicalDevice, t_defaultTexture->GetImage(), 4, 1);
							t_material->SetNeedUpdate(true);
						}
						else
						{
							t_newRenderObject->SetTexture(a_logicalDevice, t_roughness->GetImage(), 4, 1);
						}
					}
					else
					{
						t_newRenderObject->SetTexture(a_logicalDevice, t_material->GetEmpty()->GetImage(), 4, 1);
					}
					if (t_material->HasAO())
					{
						Ref<Resource::Texture> t_ao = t_material->GetAO();
						if (!t_ao->IsCreated())
						{
							t_ao->CreateImage(a_renderer);
							const Ref<Resource::Texture> t_defaultTexture = ServiceLocator::GetResourceManager()->GetResource<Resource::Texture>("Assets/Textures/DefaultTexture.png");
							t_newRenderObject->SetTexture(a_logicalDevice, t_defaultTexture->GetImage(), 5, 1);
							t_material->SetNeedUpdate(true);
						}
						else
						{
							t_newRenderObject->SetTexture(a_logicalDevice, t_ao->GetImage(), 5, 1);
						}
					}
					else
					{
						t_newRenderObject->SetTexture(a_logicalDevice, t_material->GetEmpty()->GetImage(), 5, 1);
					}
					t_newRenderObject->SetUniform(a_logicalDevice, a_physicalDevice, a_commandPool, 1, &t_material->GetMaterialValues(), sizeof(MaterialValues), 6, 1);
					t_newRenderObject->SetUniform(a_logicalDevice, a_physicalDevice, a_commandPool, 2, &t_material->GetHasTextures(), sizeof(HasMaterialTextures), 7, 1);
				}
				else if (t_material->GetType() == SKYBOX)
				{
					continue;
				}
				else
				{
					LOG_ERROR("Not other type of pipeline has been implemented");
				}
				if (t_material->GetType() != SKYBOX)
					t_newRenderObject->SetUniform(a_logicalDevice, a_physicalDevice, a_commandPool, 0, &a_updatedMatrix.at(a_indexes[i]).second, sizeof(Math::UniformMatrixs), 0, 1);

				Ref<Resource::Texture> t_albedo = t_material->GetAlbedo();
				if (t_albedo)
				{
					if (t_albedo->IsCreated())
					{
						t_newRenderObject->SetTexture(a_logicalDevice, t_albedo->GetImage(), 1, 1);

					}
					else
					{
						Resource::ResourceManager* t_resourceManager = ServiceLocator::GetResourceManager();
						const Ref<Resource::Texture> t_defaultTexture = t_material->GetType() == SKYBOX ? t_resourceManager->GetResource<Resource::Texture>("Assets/Textures/CubeMap/default_cubemap.png") : t_resourceManager->GetResource<Resource::Texture>("Assets/Textures/DefaultTexture.png");
						t_newRenderObject->SetTexture(a_logicalDevice, t_defaultTexture->GetImage(), 1, 1);
						t_albedo->CreateImage(a_renderer);
						t_material->SetNeedUpdate(true);
					}
				}
				else
				{
					Resource::ResourceManager* t_resourceManager = ServiceLocator::GetResourceManager();
					const Ref<Resource::Texture> t_defaultTexture = t_material->GetType() == SKYBOX ? t_resourceManager->GetResource<Resource::Texture>("Assets/Textures/CubeMap/default_cubemap.png") : t_resourceManager->GetResource<Resource::Texture>("Assets/Textures/DefaultTexture.png");
					t_material->SetAlbedo(t_defaultTexture);
					t_newRenderObject->SetTexture(a_logicalDevice, t_defaultTexture->GetImage(), 1, 1);
				}

				m_objectsDescriptors[a_indexes[i]]->Destroy(a_logicalDevice);
				delete m_objectsDescriptors[a_indexes[i]];

				m_objectsDescriptors[a_indexes[i]] = t_newRenderObject;
			}
		}
	}
}
