#ifndef KN_VULKAN_CMD_HPP
#define KN_VULKAN_CMD_HPP

#include "kon/container/arraylist.hpp"
#include "kon/core/allocator.hpp"
#include <vulkan/vulkan_core.h>

namespace kon {

class VulkanContext;

class VulkanCommandPool {
public:
	VulkanCommandPool(Allocator *allocator, VulkanContext *context);
	~VulkanCommandPool();

	void create(u32 queueIndex, u32 count);
	void destroy();

public:
	VkCommandPool get_pool() const { return m_pool; }

	const ArrayList<VkCommandBuffer> &get_buffer_array() const { return m_buffers; }
	VkCommandBuffer get_buffer(u32 index) const { return m_buffers[index]; }

private:
	VulkanContext *m_context;
	Allocator *m_allocator;	

	VkCommandPool m_pool;
	ArrayList<VkCommandBuffer> m_buffers;
};

}

#endif
