#ifndef KN_VULKAN_PIPELINE_HPP
#define KN_VULKAN_PIPELINE_HPP

#include "kon/container/arraylist.hpp"
#include "kon/core/allocator.hpp"
// #include "modules/graphics/vulkan/vulkan_shader.hpp"
#include <vulkan/vulkan_core.h>

namespace kon {

class VulkanContext;
class VulkanBuffer;

class VulkanPipeline {
public:
	virtual void bind_pipeline(VkCommandBuffer cmd) = 0;
	virtual void bind_descriptor_sets(VkCommandBuffer cmd, VulkanBuffer *buffer=nullptr) = 0;
	virtual void bind_push_constants(VkCommandBuffer cmd, char *data) = 0;
	virtual void draw(VkCommandBuffer cmd) = 0;
};

// ------------ BUILDERS ------------ //
class VulkanGeometryPipelineBuilder {
public:
	VulkanGeometryPipelineBuilder(Allocator *allocator, VulkanContext *context);
	~VulkanGeometryPipelineBuilder();

	void clear();
	VkPipeline build(VkPipelineLayout pipelineLayout);

	void vertex_shader(VkShaderModule vertex);
	void fragment_shader(VkShaderModule fragment);

	void input_assemply(VkPrimitiveTopology topoType);
	void rasterize(VkPolygonMode mode, VkCullModeFlags cullMode, VkFrontFace frontFace);
	void blending();
	void multisample();
	void color_format(VkFormat format);
	void depth_format(VkFormat format, VkCompareOp op, bool depthWriteEnable);

private:
	VkPipelineInputAssemblyStateCreateInfo inputAssembly;
    VkPipelineRasterizationStateCreateInfo rasterizer;
    VkPipelineColorBlendAttachmentState colorBlendAttachment;
    VkPipelineMultisampleStateCreateInfo multisampling;
    VkPipelineDepthStencilStateCreateInfo depthStencil;
    VkPipelineRenderingCreateInfo renderInfo;
    VkFormat colorAttachmentformat;

	ArrayList<VkPipelineShaderStageCreateInfo> shaderStages;

	VulkanContext *m_context;
};

}

#endif
