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
	
	void create(void* data, VkExtent3D size, VkFormat format, VkImageTiling tiling,
			VkImageUsageFlags usage, bool mipmapped = false,
			VkSampleCountFlagBits numSamples=VK_SAMPLE_COUNT_1_BIT);

	void destroy();

public:
	inline VkImage get_handle() const { return m_image; }
	inline VkExtent3D get_extent() const { return m_imageExtent; }
	inline VkImageLayout get_layout() const { return m_layout; }
	inline VkFormat get_format() const { return m_format; }

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
	VkFormat m_format;

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
	VkImageView get_handle() const { return m_view; }

private:
	VkImageView m_view { VK_NULL_HANDLE };
	VulkanContext *m_context;
};

class VulkanTexture {
public:
	VulkanTexture(VulkanContext *context);
	~VulkanTexture();

	void create(void *data, VkExtent3D extent, VkFormat format, VkImageTiling tiling,
				bool miped=false, VkSampleCountFlagBits numSamples=VK_SAMPLE_COUNT_1_BIT);

	void destroy();

	inline VulkanImageView &get_view() { return m_view; }
	inline VulkanImage &get_image() { return m_image; }
	inline VkSampler get_sampler() const { return m_imageSampler; }

private:
	VulkanContext *m_context;

	VulkanImageView m_view;
	VulkanImage m_image;
	VkSampler m_imageSampler;
};

}

#endif

