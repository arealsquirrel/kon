
#include "vulkan_buffer.hpp"
#include "modules/graphics/vulkan/vulkan_context.hpp"
#include <cstring>
#include <vulkan/vulkan_core.h>
#include <vk_mem_alloc.h>

namespace kon {

void Buffer::create_buffer(VmaAllocator allocator,
			VkDeviceSize size, VkBufferUsageFlags usage,
			VkMemoryPropertyFlags properties,
			VkBuffer &buffer, VmaAllocation &allocation, 
			VmaAllocationCreateFlags extraflags) {

	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VmaAllocationCreateInfo allocInfo = {};
	allocInfo.usage = VMA_MEMORY_USAGE_AUTO;
	allocInfo.flags = extraflags;
	allocInfo.requiredFlags = properties;
	
	vmaCreateBuffer(allocator, &bufferInfo, &allocInfo, &buffer, &allocation, nullptr);
}

void Buffer::copy_buffer(VulkanContext *context, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
	VkCommandBuffer commandBuffer = context->start_singetime_commands();

    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	context->end_singletime_commands(commandBuffer);
}

uint32_t find_memory_type(const VulkanContext *context, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(context->get_physical_device(), &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
	    if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
	        return i;
	    }
	}

	throw std::runtime_error("failed to find suitable memory type!");
}


AttributeDescription::AttributeDescription(Allocator *allocator,
		std::initializer_list<VkFormat> attributes) 
	: m_description(allocator, attributes.size()) {

	u32 offset = 0;
	for(auto format : attributes) {
		VkVertexInputAttributeDescription attribute {};
		attribute.binding = 0;
		attribute.location = m_description.get_count();
		attribute.format = format;
		attribute.offset = offset;
		offset += get_size_of_format(format);

		m_description.add(attribute);
	}
}

AttributeDescription::~AttributeDescription() = default;

VulkanVertexBuffer::VulkanVertexBuffer(VulkanContext *context, u8 *data, size_t sizeOfVertex, size_t verticies)
	: m_context(context), m_verticies(verticies) {

	VkDeviceSize size = (sizeOfVertex * verticies);
	VkBuffer stagingBuffer;
	VmaAllocation stagingBufferAllocation;
	Buffer::create_buffer(context->get_vma_allocator(),
			size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			stagingBuffer, stagingBufferAllocation);

	vmaMapMemory(m_context->get_vma_allocator(), stagingBufferAllocation, reinterpret_cast<void**>(&m_data));
	memcpy(m_data, data, sizeOfVertex * verticies);
	vmaUnmapMemory(m_context->get_vma_allocator(), stagingBufferAllocation);


	Buffer::create_buffer(context->get_vma_allocator(), size,
			VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			m_buffer, m_allocation);

	Buffer::copy_buffer(m_context, stagingBuffer, m_buffer, size);

	vmaDestroyBuffer(m_context->get_vma_allocator(), stagingBuffer, stagingBufferAllocation);
}

void VulkanVertexBuffer::bind_buffer(VkCommandBuffer cmdBuffer) {
	VkBuffer vertexBuffers[] = {m_buffer};
	VkDeviceSize offsets[] = {0};
	vkCmdBindVertexBuffers(cmdBuffer, 0, 1, vertexBuffers, offsets);
}

VulkanVertexBuffer::~VulkanVertexBuffer() {
	vmaDestroyBuffer(m_context->get_vma_allocator(), m_buffer, m_allocation);
}

VulkanIndexBuffer::VulkanIndexBuffer(VulkanContext *context, u32 *data, size_t size)
	: m_context(context), m_size(size) {
	
	VkBuffer stagingBuffer;
	VmaAllocation stagingBufferAllocation;
	Buffer::create_buffer(context->get_vma_allocator(), size,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			stagingBuffer, stagingBufferAllocation);

	vmaMapMemory(m_context->get_vma_allocator(), stagingBufferAllocation, reinterpret_cast<void**>(&m_data));
	memcpy(m_data, data, size);
	vmaUnmapMemory(m_context->get_vma_allocator(), stagingBufferAllocation);


	Buffer::create_buffer(m_context->get_vma_allocator(), size,
			VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			m_buffer, m_allocation);

	Buffer::copy_buffer(m_context, stagingBuffer, m_buffer, size);

	vmaDestroyBuffer(m_context->get_vma_allocator(), stagingBuffer, stagingBufferAllocation);
}

void VulkanIndexBuffer::draw(VkCommandBuffer cmdBuffer) {
	vkCmdDrawIndexed(cmdBuffer, m_size / sizeof(u32), 1, 0, 0, 0);
}

VulkanIndexBuffer::~VulkanIndexBuffer() {
	vmaDestroyBuffer(m_context->get_vma_allocator(), m_buffer, m_allocation);
}

void VulkanIndexBuffer::bind_buffer(VkCommandBuffer cmdBuffer) {
	vkCmdBindIndexBuffer(cmdBuffer, m_buffer, 0, VK_INDEX_TYPE_UINT32);
}

VulkanUniformBuffer::VulkanUniformBuffer(VulkanContext *context, size_t size) 
	: m_context(context), m_size(size) {

	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VmaAllocationCreateInfo allocInfo = {};
	allocInfo.usage = VMA_MEMORY_USAGE_AUTO;
	allocInfo.requiredFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	allocInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT;
	
	KN_VULKAN_ERR_CHECK(
			vmaCreateBuffer(m_context->get_vma_allocator(),
				&bufferInfo, &allocInfo, &m_buffer, &m_allocation, &m_info));
}

void VulkanUniformBuffer::write_memory(void *data) const {
	memcpy(m_info.pMappedData, data, m_size);
}

VulkanUniformBuffer::~VulkanUniformBuffer() {
	vmaDestroyBuffer(m_context->get_vma_allocator(), m_buffer, m_allocation);	
}

}

