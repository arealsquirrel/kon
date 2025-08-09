
#include "vulkan_pipeline.hpp"
#include "kon/debug/log.hpp"
#include "modules/graphics/vulkan/vulkan_context.hpp"
#include <vulkan/vulkan_core.h>

namespace kon {

VulkanGeometryPipelineBuilder::VulkanGeometryPipelineBuilder(Allocator *allocator, VulkanContext *context)
	: shaderStages(allocator), m_context(context) {
	
	clear();
}

VulkanGeometryPipelineBuilder::~VulkanGeometryPipelineBuilder() {}

void VulkanGeometryPipelineBuilder::vertex_shader(VkShaderModule vertex) {
	VkPipelineShaderStageCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	info.stage = VK_SHADER_STAGE_VERTEX_BIT;
	info.module = vertex;
	info.pName = "main";

	shaderStages.add(info);
}

void VulkanGeometryPipelineBuilder::fragment_shader(VkShaderModule fragment) {
	VkPipelineShaderStageCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	info.module = fragment;
	info.pName = "main";

	shaderStages.add(info);
}

void VulkanGeometryPipelineBuilder::clear() {
	inputAssembly = {};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;

	rasterizer = {};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    
	colorBlendAttachment = {};
    
	multisampling = {};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    
	depthStencil = {};
	depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    
	renderInfo = {};
	renderInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
}

VkPipeline VulkanGeometryPipelineBuilder::build(VkPipelineLayout pipelineLayout) {
 	VkPipelineViewportStateCreateInfo viewportState = {};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.pNext = nullptr;

    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;

    // setup dummy color blending. We arent using transparent objects yet
    // the blending is just "no blend", but we do write to the color attachment
    VkPipelineColorBlendStateCreateInfo colorBlending = {};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.pNext = nullptr;

    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;

    // completely clear VertexInputStateCreateInfo, as we have no need for it
    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {}; // = { .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO };
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

	VkGraphicsPipelineCreateInfo pipelineInfo = {}; // = { .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO };
    // connect the renderInfo to the pNext extension mechanism
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.pNext = &renderInfo;

    pipelineInfo.stageCount = (uint32_t)2;
    pipelineInfo.pStages = shaderStages.get_buffer();
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDepthStencilState = &depthStencil;
    pipelineInfo.layout = pipelineLayout;

	VkDynamicState state[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

    VkPipelineDynamicStateCreateInfo dynamicInfo = {};
    dynamicInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicInfo.pDynamicStates = &state[0];
    dynamicInfo.dynamicStateCount = 2;

    pipelineInfo.pDynamicState = &dynamicInfo;

	VkPipeline pipeline;
	KN_VULKAN_ERR_CHECK(vkCreateGraphicsPipelines(m_context->get_device(),
						VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline));

	return pipeline;
}

void VulkanGeometryPipelineBuilder::input_assemply(VkPrimitiveTopology topoType) {
	inputAssembly.topology = topoType;
	inputAssembly.primitiveRestartEnable = VK_FALSE;
}

void VulkanGeometryPipelineBuilder::rasterize(VkPolygonMode mode, VkCullModeFlags cullMode, VkFrontFace frontFace) {
	rasterizer.polygonMode = mode;
    rasterizer.lineWidth = 1.f;
	rasterizer.cullMode = cullMode;
    rasterizer.frontFace = frontFace;
}

void VulkanGeometryPipelineBuilder::blending() {
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    // no blending
    colorBlendAttachment.blendEnable = VK_FALSE;
}

void VulkanGeometryPipelineBuilder::multisample() {
	multisampling.sampleShadingEnable = VK_FALSE;
    // multisampling defaulted to no multisampling (1 sample per pixel)
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampling.minSampleShading = 1.0f;
    multisampling.pSampleMask = nullptr;
    // no alpha to coverage either
    multisampling.alphaToCoverageEnable = VK_FALSE;
    multisampling.alphaToOneEnable = VK_FALSE;
}

void VulkanGeometryPipelineBuilder::color_format(VkFormat format) {
	colorAttachmentformat = format;
    // connect the format to the renderInfo  structure
    renderInfo.colorAttachmentCount = 1;
    renderInfo.pColorAttachmentFormats = &colorAttachmentformat;
}

void VulkanGeometryPipelineBuilder::depth_format(VkFormat format) {
    renderInfo.depthAttachmentFormat = format;
	depthStencil.depthTestEnable = VK_FALSE;
    depthStencil.depthWriteEnable = VK_FALSE;
    depthStencil.depthCompareOp = VK_COMPARE_OP_NEVER;
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.stencilTestEnable = VK_FALSE;
    depthStencil.front = {};
    depthStencil.back = {};
    depthStencil.minDepthBounds = 0.f;
    depthStencil.maxDepthBounds = 1.f;
}

}

