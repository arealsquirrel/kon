#ifndef KN_VULKAN_COMPUTE_SCREEN_HPP
#define KN_VULKAN_COMPUTE_SCREEN_HPP

#include "kon/math/vector4.hpp"
#include "modules/graphics/vulkan/vulkan_pipeline.hpp"
#include <vulkan/vulkan_core.h>

namespace kon {

class VulkanContext;

/*
 * this is the compute pipeline in charge of rendering the
 * framebuffer to the screen
 */
class VulkanComputePipelineScreen : VulkanRenderPipeline {
public:
	struct alignas(128) PushConstant {
		Vector4 color1 {1.0f, 0.0f, 0.0f, 1.0f};
		Vector4 color2 {0.0f, 0.0f, 1.0f, 1.0f};
		Vector4 bs1;
		Vector4 bs2;
	};

public:
	VulkanComputePipelineScreen(VulkanContext *context);
	~VulkanComputePipelineScreen();

	void create(Allocator *allocator, VulkanShader *computeShader);
	void destroy();

public:
	void bind_pipeline(VkCommandBuffer cmd) override;
	void bind_descriptor_sets(VkCommandBuffer cmd) override;
	void bind_push_constants(VkCommandBuffer cmd, char *data) override;
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
