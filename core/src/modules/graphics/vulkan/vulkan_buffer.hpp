
#pragma once

#include "kon/container/arraylist.hpp"
#include "kon/core/allocator.hpp"
// #include "modules/graphics/graphics_module.hpp"
#include "modules/graphics/vulkan/vulkan_context.hpp"
#include "modules/graphics/vulkan/vulkan_image.hpp"
#include <cstddef>
#include <initializer_list>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

#include <vulkan/vulkan_core.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#pragma GCC diagnostic pop

namespace kon {

struct Vertex;

/*

template<typename T>
constexpr VkVertexInputBindingDescription generate_attrabute_description() {
	VkVertexInputBindingDescription bindingDescription{};
	bindingDescription.binding = 0;
	bindingDescription.stride = sizeof(T);
	bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    return bindingDescription;
}

constexpr u32 get_size_of_format(VkFormat format) {
	switch (format) {
		case VK_FORMAT_R32G32_SFLOAT: return sizeof(float) * 2;
		case VK_FORMAT_R32G32B32_SFLOAT: return sizeof(float) * 3;

		default:
			return 0;
	}
}

struct AttributeDescription {
public:
	AttributeDescription(Allocator *allocator,
			std::initializer_list<VkFormat> attributes);
	~AttributeDescription();

public:
	ArrayList<VkVertexInputAttributeDescription> m_description;
	size_t offset {0};
};
*/

class VulkanBuffer {
public:
	VulkanBuffer(VulkanContext *context);
	~VulkanBuffer();

public:
	void create(size_t allocSize, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage);
	void destroy();

	inline VkBuffer get_buffer() const { return m_buffer; }
	inline VmaAllocation get_allocation() const { return m_allocation; }
	inline VmaAllocationInfo get_allocation_info() const { return m_info; }

public:
	/*
	static void create_buffer(VmaAllocator allocator, 
			VkDeviceSize size, VkBufferUsageFlags usage,
			VkMemoryPropertyFlags properties,
			VkBuffer &buffer, VmaAllocation &allocation, 
			VmaAllocationCreateFlags extraflags=VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT);
	*/

	static void copy_buffer(VulkanContext *context, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

private:
	VulkanContext *m_context;

	VkBuffer m_buffer;
    VmaAllocation m_allocation;
    VmaAllocationInfo m_info;
};

class VulkanMeshBuffer {
public:
	VulkanMeshBuffer(VulkanContext *context);
	~VulkanMeshBuffer();

	void create(const ArrayList<Vertex> &verticies, const ArrayList<u32> &indicies);
	void destroy();

	inline VkDeviceAddress get_vertex_address() const { return m_vertexBufferAddress; };
	inline VulkanBuffer &get_vertex_buffer() { return m_vertex; }
	inline VulkanBuffer &get_index_buffer() { return m_index; }
	inline u32 get_index_count() const { return m_indexCount; }

	void bind(VkCommandBuffer cmd);
	void draw(VkCommandBuffer cmd);

private:
	VulkanContext *m_context;

	u32 m_indexCount;
	VulkanBuffer m_vertex;
	VulkanBuffer m_index;
	VkDeviceAddress m_vertexBufferAddress;
};

/*

class VulkanVertexBuffer {
public:
	VulkanVertexBuffer(VulkanContext *context, u8 *data, size_t sizeOfVertex, size_t verticies);
	~VulkanVertexBuffer();

	VkBuffer get_buffer() const { return m_buffer; }
	void bind_buffer(VkCommandBuffer cmdBuffer);

private:
	VulkanContext *m_context;

	VkBuffer m_buffer;
	VmaAllocation m_allocation;

	size_t m_verticies;

	void *m_data;
};

class VulkanIndexBuffer {
public:
	VulkanIndexBuffer(VulkanContext *context, u32 *data, size_t size);
	~VulkanIndexBuffer();

	VkBuffer get_buffer() const { return m_buffer; }

	void bind_buffer(VkCommandBuffer cmdBuffer);

	void draw(VkCommandBuffer cmdBuffer);

private:
	VulkanContext *m_context;

	void *m_data;
	VkBuffer m_buffer;
	VmaAllocation m_allocation;
	size_t m_size;
};

class VulkanUniformBuffer {
public:
	VulkanUniformBuffer(VulkanContext *context, size_t size);
	~VulkanUniformBuffer();

	void write_memory(void *data) const;
	VkBuffer get_buffer() const { return m_buffer; }

private:
	VulkanContext *m_context;

	size_t m_size;
	VkBuffer m_buffer;
	VmaAllocation m_allocation;
	VmaAllocationInfo m_info;
};
*/

}
