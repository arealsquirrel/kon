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
	virtual void bind_push_constants(VkCommandBuffer cmd, char *data) = 0;
	virtual void draw(VkCommandBuffer cmd) = 0;
};

}

#endif
