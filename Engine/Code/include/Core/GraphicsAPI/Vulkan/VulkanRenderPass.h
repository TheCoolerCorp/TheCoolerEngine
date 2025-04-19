#ifndef VULKANRENDERPASS_H
#define VULKANRENDERPASS_H

#include <functional>
#include <map>
#include <set>
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

			struct RecordRenderPassinfo
			{
				VkCommandBuffer commandBuffer;
				uint32_t imageIndex;
				uint32_t currentFrame;
				VulkanSwapchain* swapChain;
				const VulkanGraphicPipeline* graphicPipeline;
				const GamePlay::Camera* camera;
			};
			enum RenderPassFlags : std::uint8_t
			{
				FLAG_VK_RHI_OVERRIDE_DEFAULT_RENDERPASS = 0,
			};

			class VulkanRenderPassManager : public RHI::IRenderPass
			{
			public:
				ENGINE_API ~VulkanRenderPassManager() override = default;
				ENGINE_API VulkanRenderPassManager* CastVulkan() override { return this; }

				// --- Initialization ---
				ENGINE_API void Create(Renderer* renderer) override;
				ENGINE_API void CreateDefaultRenderPass(Renderer* renderer);
				ENGINE_API void Destroy(RHI::ILogicalDevice* logicalDevice) override;

				// --- Recording ---
				ENGINE_API void RecordRenderPasses(const RecordRenderPassinfo& a_info,
					const std::vector<Core::RHI::IRenderObject*>& a_renderObjects,
					const std::vector<Core::RHI::IBuffer*>& a_vertexBuffers,
					const std::vector<Core::RHI::IBuffer*>& a_indexBuffers,
					const std::vector<uint32_t>& a_nbIndices);

				ENGINE_API void RunSceneRenderPass(const RecordRenderPassinfo& a_info,
					const std::vector<Core::RHI::IRenderObject*>& a_renderObjects,
					const std::vector<Core::RHI::IBuffer*>& a_vertexBuffers,
					const std::vector<Core::RHI::IBuffer*>& a_indexBuffers,
					const std::vector<uint32_t>& a_nbIndices);

				// --- Dependency Resolution ---
				ENGINE_API void ResolveDependencies(VulkanRenderPass* pass,
					std::set<VulkanRenderPass*>& visited,
					std::vector<VulkanRenderPass*>& sorted);

				// --- Setters ---
				ENGINE_API void SetSceneRenderPass(VulkanRenderPass* renderPass);
				ENGINE_API void AddRenderPass(VulkanRenderPass* renderPass);

				// --- Getters ---
				[[nodiscard]] ENGINE_API VulkanRenderPass* GetSceneRenderPass() const { return m_sceneRenderPass; }

				// --- Static Flags ---
				ENGINE_API static bool HasFlag(RenderPassFlags a_flag);
				ENGINE_API static void AddFlag(RenderPassFlags a_flag);
				ENGINE_API static void RemoveFlag(RenderPassFlags a_flag);

				void InsertPipelineBarrier(const VulkanRenderPass* a_pass, const VulkanRenderPass* a_dependentPass, VkCommandBuffer a_buffer) const;
			private:
				Renderer* m_renderer = nullptr;
				VulkanRenderPass* m_sceneRenderPass = nullptr;
				std::vector<VulkanRenderPass*> m_renderPasses;

				// Static configuration flags (used globally)
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
				VkImageLayout dependencyImageLayoutOverride = VK_IMAGE_LAYOUT_UNDEFINED;
				VkExtent2D extent;
				bool setViewportAndScissor = false;
				bool useSwapChainFramebuffers = false;
				bool createOwnFramebuffers = true;
			};

			struct AttachmentResource {
				VkImage image = VK_NULL_HANDLE;
				VkDeviceMemory memory = VK_NULL_HANDLE;
				VkImageView view = VK_NULL_HANDLE;
			};

			/**
			* @brief VulkanRenderPass is a wrapper around the Vulkan render pass object.
			* It handles the creation and destruction of a render pass, as well as the
			* framebuffers and attachments.
			*/
			class VulkanRenderPass
			{
			public:
				ENGINE_API VulkanRenderPass(VkDevice device, Renderer* renderer);
				ENGINE_API ~VulkanRenderPass();

				// --- Creation ---
				ENGINE_API void Create(const RenderPassConfig& config);
				ENGINE_API void CreateAttachments();
				ENGINE_API void CreateFramebuffers(); // Uses internally defined attachments
				ENGINE_API void CreateFramebuffers(const std::vector<std::vector<VkImageView>>& views); // Uses provided views
				ENGINE_API void SetParent(VulkanRenderPassManager* a_parent) { m_parent = a_parent; }

				// --- Lifecycle ---
				ENGINE_API void Destroy();

				// --- Recording ---
				ENGINE_API void RecordRenderPass(RecordRenderPassinfo info,
					const std::vector<Core::RHI::IRenderObject*>& renderObjects,
					const std::vector<Core::RHI::IBuffer*>& vertexBuffers,
					const std::vector<Core::RHI::IBuffer*>& indexBuffers,
					const std::vector<uint32_t>& nbIndices);

				ENGINE_API void Begin(VkCommandBuffer cmd, uint32_t imageIndex, uint32_t currentFrame, VkSubpassContents contents = VK_SUBPASS_CONTENTS_INLINE);
				ENGINE_API void End(VkCommandBuffer cmd);

				// --- Draw Control ---
				ENGINE_API void SetDrawFunc(std::function<void(RecordRenderPassinfo,
					const std::vector<Core::RHI::IRenderObject*>&,
					const std::vector<Core::RHI::IBuffer*>&,
					const std::vector<Core::RHI::IBuffer*>&,
					const std::vector<uint32_t>&)> a_func);

				// --- Dependencies ---
				ENGINE_API void AddDependency(VulkanRenderPass* dependency) { m_dependencies.push_back(dependency); }
				[[nodiscard]] ENGINE_API const std::vector<VulkanRenderPass*>& GetDependencies() { return m_dependencies; }
				[[nodiscard]] ENGINE_API bool HasDependency() const { return !m_dependencies.empty(); }

				// --- State Transition ---
				ENGINE_API void TransitionImageLayout(VkImageLayout newImageLayout);

				// --- Getters ---
				[[nodiscard]] ENGINE_API VkRenderPass GetRenderPass() const { return m_renderPass; }
				[[nodiscard]] ENGINE_API const std::vector<VkFramebuffer>& GetFramebuffers() const { return m_framebuffers; }
				[[nodiscard]] ENGINE_API const std::vector<AttachmentResource>& GetAttachmentResources() const { return m_attachmentResources; }
				[[nodiscard]] ENGINE_API VkSampler GetSampler() const { return m_sampler; }
				[[nodiscard]] ENGINE_API const RenderPassConfig& GetConfig() const { return m_config; }
			private:
				// --- Vulkan handles and references ---
				VkDevice m_device;
				Renderer* m_renderer;
				VulkanRenderPassManager* m_parent;

				// --- Configuration and pipeline ---
				RenderPassConfig m_config;
				VkRenderPass m_renderPass = VK_NULL_HANDLE;

				// --- Attachments ---
				std::vector<AttachmentResource> m_attachmentResources;
				AttachmentResource m_depthAttachmentResource;
				bool m_isDepth = false;
				VkSampler m_sampler = VK_NULL_HANDLE;
				std::vector<VkFramebuffer> m_framebuffers;

				// --- Rendering logic ---
				std::function<void(RecordRenderPassinfo,
					const std::vector<Core::RHI::IRenderObject*>&,
					const std::vector<Core::RHI::IBuffer*>&,
					const std::vector<Core::RHI::IBuffer*>&,
					const std::vector<uint32_t>&)> m_drawFunc;

				// --- Dependencies ---
				std::vector<VulkanRenderPass*> m_dependencies;

				// --- Helpers ---
				void CreateDepthAttachment(const RenderPassAttachment& attachment);
				void CreateAttachment(const RenderPassAttachment& attachment, uint32_t index);
			};
		}
	}
}
#endif