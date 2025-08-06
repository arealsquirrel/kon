
#include "vulkan_cmd.hpp"
#include "modules/graphics/vulkan/vulkan_context.hpp"
#include <vulkan/vulkan_core.h>

namespace kon {

VulkanCommandPool::VulkanCommandPool(Allocator *allocator, VulkanContext *context)
	: m_context(context), m_allocator(allocator), m_buffers(allocator) {}

VulkanCommandPool::~VulkanCommandPool() = default;

void VulkanCommandPool::create(u32 queueIndex, u32 count) {
	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = queueIndex;

	KN_VULKAN_ERR_CHECK(
			vkCreateCommandPool(m_context->get_device(), &poolInfo, nullptr, &m_pool))

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = m_pool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = count;
	m_buffers.resize(count);
	KN_VULKAN_ERR_CHECK(vkAllocateCommandBuffers(m_context->get_device(),
				&allocInfo, m_buffers.get_buffer()));
}

void VulkanCommandPool::destroy() {
	vkDestroyCommandPool(m_context->get_device(),
						 m_pool, nullptr);
}

}

