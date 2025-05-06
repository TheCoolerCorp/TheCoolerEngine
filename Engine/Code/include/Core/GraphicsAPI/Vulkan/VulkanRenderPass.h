#ifndef VULKANRENDERPASS_H
#define VULKANRENDERPASS_H


#include <functional>
#include <set>
#include <unordered_map>
#include <vulkan/vulkan_core.h>

#include "EngineExport.h"
#include "Core/Interfaces/IRenderPass.h"

namespace Engine
{
	namespace GamePlay
	{
		class Scene;
	}

	namespace Core
	{
		namespace RHI
		{
			class IObjectDescriptor;
			class IBuffer;
			enum DescriptorSetPipelineTarget : int;
		}

		class Renderer;

		namespace GraphicsAPI
		{
			class VulkanRenderPass;
			class VulkanGraphicPipeline;

			struct RecordRenderPassinfo
			{
				Renderer* renderer;
				GamePlay::Scene* scene;
				int commandPoolIndex;
				uint32_t imageIndex;
				uint32_t currentFrame;
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
				ENGINE_API void Create(Renderer* a_renderer) override;
				ENGINE_API void CreateDefaultRenderPass(Renderer* a_renderer);
				ENGINE_API void Destroy(RHI::ILogicalDevice* a_logicalDevice) override;

				// --- Recording ---
				ENGINE_API void RecordRenderPasses(RecordRenderPassinfo& a_info,
					std::unordered_map<RHI::DescriptorSetPipelineTarget, std::vector<RHI::IBuffer*>>& a_vertexBuffers,
					std::unordered_map<RHI::DescriptorSetPipelineTarget, std::vector<RHI::IBuffer*>>& a_indexBuffers,
					std::unordered_map<RHI::DescriptorSetPipelineTarget, std::vector<uint32_t>>& a_nbIndices,
					std::unordered_map<RHI::DescriptorSetPipelineTarget, std::vector<RHI::IObjectDescriptor*>>& a_descriptors);

				ENGINE_API void RunSceneRenderPass(RecordRenderPassinfo& a_info,
					std::unordered_map<RHI::DescriptorSetPipelineTarget, std::vector<RHI::IBuffer*>>& a_vertexBuffers,
					std::unordered_map<RHI::DescriptorSetPipelineTarget, std::vector<RHI::IBuffer*>>& a_indexBuffers,
					std::unordered_map<RHI::DescriptorSetPipelineTarget, std::vector<uint32_t>>& a_nbIndices,
					std::unordered_map<RHI::DescriptorSetPipelineTarget, std::vector<RHI::IObjectDescriptor*>>& a_descriptors);

				// --- Setters ---
				ENGINE_API void SetSceneRenderPass(VulkanRenderPass* a_renderPass);
				ENGINE_API void AddRenderPass(VulkanRenderPass* a_renderPass);

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
				bool setResizeCallback = true;
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
				ENGINE_API VulkanRenderPass(VkDevice a_device, Renderer* a_renderer);
				ENGINE_API ~VulkanRenderPass();

				// --- Creation ---
				ENGINE_API void Create(const RenderPassConfig& a_config);
				ENGINE_API void CreateAttachments();
				ENGINE_API void CreateFramebuffers(); // Uses internally defined attachments
				ENGINE_API void CreateFramebuffers(const std::vector<std::vector<VkImageView>>& a_views); // Uses provided views
				ENGINE_API void SetParent(VulkanRenderPassManager* a_parent) { m_parent = a_parent; }
				ENGINE_API void RecreateFrameBuffer(VkExtent2D a_extent);
				ENGINE_API void AddPipelineDependency(VulkanGraphicPipeline* a_pipeline) { m_graphicPipelines.push_back(a_pipeline); }	

				// --- Lifecycle ---
				ENGINE_API void Destroy();
				ENGINE_API void DestroyFramebuffers();
				ENGINE_API void DestroyAttachments();

				// --- Recording ---
				ENGINE_API void RecordRenderPass(RecordRenderPassinfo& a_info,
					std::unordered_map<RHI::DescriptorSetPipelineTarget, std::vector<RHI::IBuffer*>>& a_vertexBuffers,
					std::unordered_map<RHI::DescriptorSetPipelineTarget, std::vector<RHI::IBuffer*>>& a_indexBuffers,
					std::unordered_map<RHI::DescriptorSetPipelineTarget, std::vector<uint32_t>>& a_nbIndices,
					std::unordered_map<RHI::DescriptorSetPipelineTarget, std::vector<RHI::IObjectDescriptor*>>& a_descriptors);

				ENGINE_API void Begin(VkCommandBuffer a_cmd, uint32_t a_imageIndex, uint32_t a_currentFrame, VkSubpassContents a_contents = VK_SUBPASS_CONTENTS_INLINE) const;
				ENGINE_API void End(VkCommandBuffer a_cmd);

				// --- Draw Control ---
				ENGINE_API void SetDrawFunc(std::function<void(RecordRenderPassinfo&,
				                                               std::unordered_map<RHI::DescriptorSetPipelineTarget, std::vector<RHI::IBuffer*>>&,
				                                               std::unordered_map<RHI::DescriptorSetPipelineTarget, std::vector<RHI::IBuffer*>>&,
				                                               std::unordered_map<RHI::DescriptorSetPipelineTarget, std::vector<uint32_t>>&,
				                                               std::unordered_map<RHI::DescriptorSetPipelineTarget, std::vector<RHI::IObjectDescriptor*>>&)> a_func);

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
				std::vector<VulkanGraphicPipeline*> m_graphicPipelines;

				// --- Attachments ---
				std::vector<AttachmentResource> m_attachmentResources;
				AttachmentResource m_depthAttachmentResource;
				bool m_isDepth = false;
				VkSampler m_sampler = VK_NULL_HANDLE;
				std::vector<VkFramebuffer> m_framebuffers;

				// --- Rendering logic ---
				std::function<void(RecordRenderPassinfo&,
					std::unordered_map<RHI::DescriptorSetPipelineTarget, std::vector<RHI::IBuffer*>>&,
					std::unordered_map<RHI::DescriptorSetPipelineTarget, std::vector<RHI::IBuffer*>>&,
					std::unordered_map<RHI::DescriptorSetPipelineTarget, std::vector<uint32_t>>&,
					std::unordered_map<RHI::DescriptorSetPipelineTarget, std::vector<RHI::IObjectDescriptor*>>&)> m_drawFunc;

				// --- Helpers ---
				void CreateDepthAttachment(const RenderPassAttachment& a_attachment);
				void CreateAttachment(const RenderPassAttachment& a_attachment, uint32_t a_index);

				void RunPipelineDrawFuncs(RecordRenderPassinfo& a_info,
					std::unordered_map<RHI::DescriptorSetPipelineTarget, std::vector<RHI::IBuffer*>>& a_vertexBuffers,
					std::unordered_map<RHI::DescriptorSetPipelineTarget, std::vector<RHI::IBuffer*>>& a_indexBuffers,
					std::unordered_map<RHI::DescriptorSetPipelineTarget, std::vector<uint32_t>>& a_nbIndices,
					std::unordered_map<RHI::DescriptorSetPipelineTarget, std::vector<RHI::IObjectDescriptor*>>& a_descriptors);
			};
		}
	}
}
#endif