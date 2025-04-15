#ifndef VULKANRENDERPASS_H
#define VULKANRENDERPASS_H

#include <functional>
#include <map>
#include <utility>

#include "EngineExport.h"
#include "VulkanGraphicPipeline.h"
#include "VulkanSwapChain.h"
#include "Core/GraphicsAPI/Vulkan/VulkanUtils.h"
#include "Core/Interfaces/IRenderPass.h"
#include "GamePlay/Others/Camera.h"

namespace Engine::Core
{
	class Renderer;
}

namespace Engine
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			class VulkanRenderPass;
			struct RecordRenderPassinfo;

			enum RenderPassFlags : std::uint8_t
			{
				FLAG_VK_RHI_OVERRIDE_DEFAULT_RENDERPASS = 0,
			};

			class VulkanRenderPassManager : public RHI::IRenderPass
			{
			public:
				ENGINE_API ~VulkanRenderPassManager() override = default;
				ENGINE_API VulkanRenderPassManager* CastVulkan() override { return this; }

				ENGINE_API void Create(Renderer* renderer) override;
				ENGINE_API void Destroy(RHI::ILogicalDevice* a_logicalDevice) override;

				ENGINE_API void CreateDefaultRenderPass(Renderer* a_renderer);

				ENGINE_API void RunSceneRenderPass(RecordRenderPassinfo a_info, const std::vector<Core::RHI::IRenderObject*>& a_renderObjects, const std::vector<Core::RHI::IBuffer*>& a_vertexBuffers, const std::vector<Core::RHI::IBuffer*>& a_indexBuffers, const std::vector<uint32_t>& a_nbIndices);

				//getters
				[[nodiscard]] ENGINE_API VulkanRenderPass* GetSceneRenderPass() const { return m_sceneRenderPass; }

				//setters
				ENGINE_API void SetSceneRenderPass(VulkanRenderPass* a_renderPass) { m_sceneRenderPass = a_renderPass; }
				ENGINE_API void AddRenderPass(VulkanRenderPass* a_renderPass, int a_index) { m_renderPasses[a_index].push_back(a_renderPass); }

				ENGINE_API static bool HasFlag(RenderPassFlags a_flag)
				{
					for (const RenderPassFlags flag : m_renderPassFlags)
					{
						if (flag == a_flag)
							return true;
					}
					return false;
				}
				ENGINE_API static void AddFlag(RenderPassFlags a_flag)
				{
					if (HasFlag(a_flag))
						return;
					m_renderPassFlags.push_back(a_flag);
				}
				ENGINE_API static void RemoveFlag(RenderPassFlags a_flag)
				{
					for (auto it = m_renderPassFlags.begin(); it != m_renderPassFlags.end(); ++it)
					{
						if (*it == a_flag)
						{
							m_renderPassFlags.erase(it);
							return;
						}
					}
				}
			private:
				Renderer* m_renderer;

				VulkanRenderPass* m_sceneRenderPass;
				std::map<int, std::vector<VulkanRenderPass*>> m_renderPasses;

				//static vector allows us to add/remove flags before the manager is even initialised,
				//allowing us to better control its behaviour (like preventing default renderpass creation because we are making our own)
				static std::vector<RenderPassFlags> m_renderPassFlags;
			};

			struct RenderPassAttachment
			{
				VkFormat format;
				VkImageLayout initialLayout;
				VkImageLayout finalLayout;
				VkAttachmentLoadOp loadOp;
				VkAttachmentStoreOp storeOp;
				VkImageAspectFlags aspectMask;
				bool isDepth = false;
			};

			struct SubpassConfig
			{
				std::vector<uint32_t> colorAttachmentIndices;
				int32_t depthAttachmentIndex = -1;
			};

			struct RenderPassConfig
			{
				std::vector<RenderPassAttachment> attachments;
				std::vector<SubpassConfig> subpasses;
				std::vector<VkSubpassDependency> dependencies;
				VkExtent2D extent;
				bool setViewportAndScissor;
				bool useSwapChainFramebuffers = false;
			};

			struct AttachmentResource {
				VkImage image;
				VkDeviceMemory memory;
				VkImageView view;
			};

			struct RecordRenderPassinfo
			{
				VkCommandBuffer commandBuffer;
				uint32_t imageIndex;
				VulkanSwapchain* swapChain;
				const VulkanGraphicPipeline* graphicPipeline;
				const GamePlay::Camera* camera;
			};

			class VulkanRenderPass
			{
			public:
				VulkanRenderPass(VkDevice device, Renderer* renderer);
				~VulkanRenderPass();

				ENGINE_API void Create(const RenderPassConfig& config);
				ENGINE_API void CreateFramebuffers(const std::vector<std::vector<VkImageView>>& a_views);
				ENGINE_API void SetFramebuffers(const std::vector<VkFramebuffer>& a_buffers);
				ENGINE_API void CreateAttachments();
				
				ENGINE_API void TransitionImageLayout(VkImageLayout newImageLayout);

				ENGINE_API void RecordRenderPass(RecordRenderPassinfo a_info, const std::vector<Core::RHI::IRenderObject*>& a_renderObjects, const std::vector<Core::RHI::IBuffer*>& a_vertexBuffers, const std::vector<Core::RHI::IBuffer*>& a_indexBuffers, const std::vector<uint32_t>& a_nbIndices);
				ENGINE_API void Begin(VkCommandBuffer cmd, uint32_t imageIndex, VkSubpassContents contents = VK_SUBPASS_CONTENTS_INLINE);
				ENGINE_API void End(VkCommandBuffer cmd);

				ENGINE_API void SetDrawFunc(std::function<void(RecordRenderPassinfo, const std::vector<Core::RHI::IRenderObject*>&, const std::vector<Core::RHI::IBuffer*>&, const std::vector<Core::RHI::IBuffer*>&, const std::vector<uint32_t>&)> a_func) { m_drawFunc = std::move(a_func); }

				ENGINE_API void Destroy();

				//getters
				[[nodiscard]] ENGINE_API  VkRenderPass GetRenderPass() const { return m_renderPass; }
				[[nodiscard]] ENGINE_API const std::vector<VkFramebuffer>& GetFramebuffers() const { return m_framebuffers; }
				[[nodiscard]] ENGINE_API const std::vector<AttachmentResource>& GetAttachmentResources() const { return m_AttachmentResources; }


			private:
				VkDevice m_device;
				Renderer* m_renderer;

				RenderPassConfig m_config;
				VkRenderPass m_renderPass;
				VkPipeline m_pipeline = VK_NULL_HANDLE;

				std::vector<VkFramebuffer> m_framebuffers;
				//optional, only if writing to image or texture, not swapchain
				std::vector<AttachmentResource> m_AttachmentResources;
				AttachmentResource m_depthAttachmentResource; //we only need one depth attachment for now

				//function that will be called to draw the renderpass, lambda to allow the user to define it
				std::function<void(RecordRenderPassinfo, const std::vector<Core::RHI::IRenderObject*>&, const std::vector<Core::RHI::IBuffer*>&, const std::vector<Core::RHI::IBuffer*>&, const std::vector<uint32_t>&)> m_drawFunc;

				void CreateDepthAttachment(const RenderPassAttachment& a_attachment);
				void CreateAttachment(const RenderPassAttachment& a_attachment, uint32_t i);
			};
		}
	}
}
#endif