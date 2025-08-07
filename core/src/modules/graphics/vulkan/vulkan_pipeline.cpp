
#include "vulkan_pipeline.hpp"
#include "modules/graphics/vulkan/vulkan_context.hpp"
#include <vulkan/vulkan_core.h>

namespace kon {

VulkanComputePipeline::VulkanComputePipeline(VulkanContext *context)
	: m_context(context) {}

VulkanComputePipeline::~VulkanComputePipeline() = default;

void VulkanComputePipeline::create(Allocator *allocator, VulkanShader *computeShader) {

	{
		DescriptorLayoutBuilder builder {{allocator}};
		builder.add_binding(0, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE);
		m_descriptorLayout = builder.build(m_context->get_device(), VK_SHADER_STAGE_COMPUTE_BIT);
	}

	m_descriptors = m_context->get_descriptor_allocator().allocate(m_context->get_device(), m_descriptorLayout);	

	VkDescriptorImageInfo imgInfo{};
	imgInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
	imgInfo.imageView = m_context->get_render_image_view();
	
	VkWriteDescriptorSet drawImageWrite = {};
	drawImageWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	drawImageWrite.pNext = nullptr;
	
	drawImageWrite.dstBinding = 0;
	drawImageWrite.dstSet = m_descriptors;
	drawImageWrite.descriptorCount = 1;
	drawImageWrite.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
	drawImageWrite.pImageInfo = &imgInfo;

	vkUpdateDescriptorSets(m_context->get_device(), 1, &drawImageWrite, 0, nullptr);

	VkPipelineLayoutCreateInfo computeLayout{};
	computeLayout.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	computeLayout.pNext = nullptr;
	computeLayout.pSetLayouts = &m_descriptorLayout;
	computeLayout.setLayoutCount = 1;

	KN_VULKAN_ERR_CHECK(vkCreatePipelineLayout(m_context->get_device(), &computeLayout, nullptr, &m_layout));

	VkPipelineShaderStageCreateInfo stageinfo{};
	stageinfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	stageinfo.pNext = nullptr;
	stageinfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
	stageinfo.module = computeShader->get_shader();
	stageinfo.pName = "main";

	VkComputePipelineCreateInfo computePipelineCreateInfo{};
	computePipelineCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
	computePipelineCreateInfo.pNext = nullptr;
	computePipelineCreateInfo.layout = m_layout;
	computePipelineCreateInfo.stage = stageinfo;
	
	KN_VULKAN_ERR_CHECK(vkCreateComputePipelines(m_context->get_device(),VK_NULL_HANDLE,1,&computePipelineCreateInfo, nullptr, &m_pipeline));
}

void VulkanComputePipeline::destroy() {
	vkDestroyDescriptorSetLayout(m_context->get_device(), m_descriptorLayout, nullptr);
	vkDestroyPipelineLayout(m_context->get_device(), m_layout, nullptr);
	vkDestroyPipeline(m_context->get_device(), m_pipeline, nullptr);
}

void VulkanComputePipeline::bind_pipeline(VkCommandBuffer cmd) {
	vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_COMPUTE, m_pipeline);
}

void VulkanComputePipeline::bind_descriptor_sets(VkCommandBuffer cmd) {
	vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_COMPUTE, m_layout, 0, 1, &m_descriptors, 0, nullptr);
}

void VulkanComputePipeline::draw(VkCommandBuffer cmd) {
	vkCmdDispatch(cmd, std::ceil(500 / 16.0), std::ceil(500 / 16.0), 1);
}

}

