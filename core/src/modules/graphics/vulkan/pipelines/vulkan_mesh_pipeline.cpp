
#include "vulkan_mesh_pipeline.hpp"
#include "kon/debug/log.hpp"
#include "kon/resource/resource_shader.hpp"
#include "modules/graphics/vulkan/vulkan_buffer.hpp"
#include "modules/graphics/vulkan/vulkan_descriptors.hpp"
#include "modules/graphics/vulkan/vulkan_pipeline.hpp"
#include <vulkan/vulkan_core.h>
#include <kon/engine/engine.hpp>
#include <modules/graphics/vulkan/vulkan_context.hpp>
#include <modules/graphics/vulkan/vulkan_shader.hpp>

namespace kon {

VulkanMeshPipeline::VulkanMeshPipeline(Engine *engine, VulkanContext *context)
	: m_context(context), m_engine(engine) {}

VulkanMeshPipeline::~VulkanMeshPipeline() {

}

void VulkanMeshPipeline::create(Allocator *allocator) {

	{
		DescriptorLayoutBuilder builder{{allocator}};
		builder.add_binding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
		m_descriptorLayout = builder.build(m_context->get_device(),
				VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT);
	}


	VkPushConstantRange pushConstant{};
	pushConstant.offset = 0;
	pushConstant.size = 128;
	pushConstant.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	VkPipelineLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	layoutInfo.pNext = nullptr;
	// layoutInfo.pSetLayouts = &m_descriptorLayout;
	layoutInfo.setLayoutCount = 0;
	layoutInfo.pPushConstantRanges = &pushConstant;
	layoutInfo.pushConstantRangeCount = 1;
	
	KN_VULKAN_ERR_CHECK(vkCreatePipelineLayout(m_context->get_device(),
				&layoutInfo, nullptr, &m_layout));

	VulkanGeometryPipelineBuilder builder(allocator, m_context);
	builder.input_assemply(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
	builder.rasterize(VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, VK_FRONT_FACE_CLOCKWISE);
	builder.blending();
	builder.multisample();
	builder.color_format(m_context->get_renderimage_format());
	builder.depth_format(m_context->get_depth_image().get_format(), VK_COMPARE_OP_GREATER_OR_EQUAL, VK_TRUE);

	auto &cache = m_engine->get_resource_cache();
	cache.load_resource("triangle.vert.spv");
	cache.load_resource("triangle.frag.spv");

	auto *vertexShaderSrc = cache.get_resource<ResourceShader>("triangle.vert.spv");
	auto *fragmentShaderSrc  = cache.get_resource<ResourceShader>("triangle.frag.spv");

	VulkanShader vertexShader(m_context);
	VulkanShader fragmentShader(m_context);

	vertexShader.create(vertexShaderSrc->get_shader_code(), vertexShaderSrc->get_size());
	fragmentShader.create(fragmentShaderSrc->get_shader_code(), fragmentShaderSrc->get_size());
	
	builder.fragment_shader(fragmentShader.get_shader());
	builder.vertex_shader(vertexShader.get_shader());

	m_pipeline = builder.build(m_layout);
	
	vertexShader.destroy();
	fragmentShader.destroy();
}

void VulkanMeshPipeline::destroy() {
	vkDestroyDescriptorSetLayout(m_context->get_device(), m_descriptorLayout, nullptr);
	vkDestroyPipelineLayout(m_context->get_device(), m_layout, nullptr);
	vkDestroyPipeline(m_context->get_device(), m_pipeline, nullptr);
}

void VulkanMeshPipeline::bind_pipeline(VkCommandBuffer cmd) {
	vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);
	
	//set dynamic viewport and scissor
	VkViewport viewport = {};
	viewport.x = 0;
	viewport.y = 0;
	viewport.width = 500;
	viewport.height = 500;
	viewport.minDepth = 0.f;
	viewport.maxDepth = 1.f;

	vkCmdSetViewport(cmd, 0, 1, &viewport);

	VkRect2D scissor = {};
	scissor.offset.x = 0;
	scissor.offset.y = 0;
	scissor.extent.width = 500;
	scissor.extent.height = 500;

	
	vkCmdSetScissor(cmd, 0, 1, &scissor);
}

void VulkanMeshPipeline::bind_descriptor_sets(VkCommandBuffer, VulkanBuffer *buffer) {
	// gpuSceneDataBuffer(m_context);
	// gpuSceneDataBuffer.create(sizeof(SceneData), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU);

	//m_context->get_framedata().deletionQueue.push_function([&]() {
	//	gpuSceneDataBuffer.destroy();
	// });

	//write the buffer
	SceneData* sceneUniformData = (SceneData*)buffer->get_allocation_info().pMappedData;
	*sceneUniformData = m_sceneData;

	//create a descriptor set that binds that buffer and update it
	VkDescriptorSet globalDescriptor = m_context->get_frame_descriptor().allocate(m_descriptorLayout, nullptr);

	DescriptorWriter writer(m_engine->get_allocator_dynamic(), m_context);
	writer.write_buffer(0, buffer->get_buffer(), sizeof(SceneData), 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
	writer.update_set(globalDescriptor);
}

void VulkanMeshPipeline::bind_push_constants(VkCommandBuffer cmd, char *data) {
	vkCmdPushConstants(cmd, m_layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstants), data);
}

void VulkanMeshPipeline::draw(VkCommandBuffer) {
}

}

