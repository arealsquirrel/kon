#ifndef KN_VULKAN_IMAGE_HPP
#define KN_VULKAN_IMAGE_HPP

#include "vk_mem_alloc.h"
#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace kon
{

class VulkanContext;

class VulkanImage {
public:
	VulkanImage(VulkanContext *context);
	~VulkanImage();

public:
	void create(VkExtent3D extent, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
				uint32_t mipLevels=1, VkSampleCountFlagBits numSamples=VK_SAMPLE_COUNT_1_BIT);
	void destroy();

public:
	VkImage get_handle() { return m_image; }
	VkExtent3D get_extent() const { return m_imageExtent; }
	VkImageLayout get_layout() const { return m_layout; }

public:
	static void transition_image(VkCommandBuffer cmd, VkImage image,
		VkImageLayout currentLayout, VkImageLayout newLayout);

	static void copy_image_to_image(VkCommandBuffer cmd,
			VkImage source, VkImage destination,
			VkExtent2D srcSize, VkExtent2D dstSize);
	
	static VkImageSubresourceRange image_subresource_range(VkImageAspectFlags aspectMask);

private:
	VkImage m_image { VK_NULL_HANDLE };
	VmaAllocation m_allocation;
	VkImageLayout m_layout;
	VkExtent3D m_imageExtent;

	VulkanContext *m_context;
};

class VulkanImageView {
public:
	VulkanImageView(VulkanContext *context);
	~VulkanImageView();

public:
	void create(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels=1);
	void destroy();

public:
	VkImageView get_handle() { return m_view; }

private:
	VkImageView m_view { VK_NULL_HANDLE };
	VulkanContext *m_context;
};

}

#endif

