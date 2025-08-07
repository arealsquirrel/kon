#ifndef KN_VULKAN_PIPELINE_HPP
#define KN_VULKAN_PIPELINE_HPP

#include "kon/core/allocator.hpp"
#include "modules/graphics/vulkan/vulkan_shader.hpp"
#include <vulkan/vulkan_core.h>

namespace kon {

class VulkanContext;

class VulkanRenderPipeline {
public:
	virtual void bind_pipeline(VkCommandBuffer cmd) = 0;
	virtual void bind_descriptor_sets(VkCommandBuffer cmd) = 0;
	virtual void draw(VkCommandBuffer cmd) = 0;
};

class VulkanComputePipeline : VulkanRenderPipeline {
public:
	VulkanComputePipeline(VulkanContext *context);
	~VulkanComputePipeline();

	void create(Allocator *allocator, VulkanShader *computeShader);
	void destroy();

public:
	void bind_pipeline(VkCommandBuffer cmd) override;
	void bind_descriptor_sets(VkCommandBuffer cmd) override;
	void draw(VkCommandBuffer cmd) override;

private:
	VkPipelineLayout m_layout;
	VkPipeline m_pipeline;
	VkDescriptorSet m_descriptors;
	VkDescriptorSetLayout m_descriptorLayout;

	VulkanContext *m_context;
};

}

#endif
