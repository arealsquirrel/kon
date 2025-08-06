#ifndef KN_VULKAN_SWAPCHAIN_HPP
#define KN_VULKAN_SWAPCHAIN_HPP

#include "kon/container/arraylist.hpp"
#include "kon/core/allocator.hpp"
#include <vulkan/vulkan_core.h>
namespace kon {

class VulkanContext;

class VulkanSwapchain {
public:
	VulkanSwapchain(Allocator *allocator, VulkanContext *context);
	~VulkanSwapchain();

	void create(u32 width, u32 height);
	void destroy();

	u32 aquire_next_image(VkSemaphore s);

public:
	VkSwapchainKHR get_swapchain() const { return m_swapchain; }

	const ArrayList<VkImage> &get_image_array() const { return m_images; }
	inline VkImage get_image(u32 index) const { return m_images[index]; }

	const ArrayList<VkImageView> &get_view_array() const { return m_views; }
	inline VkImageView get_view(u32 index) const { return m_views[index]; }

	inline VkSemaphore get_semaphore(u32 index) const { return m_submitSemaphores[index]; }

	VkExtent2D get_extent() const { return m_extent; }
	VkPresentModeKHR get_present_mode() const { return m_presentMode; }
	VkSurfaceFormatKHR get_format() const { return m_format; }

private:
	Allocator *m_allocator;
	VulkanContext *m_context;

	VkSwapchainKHR m_swapchain;
	ArrayList<VkImage> m_images;
	ArrayList<VkImageView> m_views;
	ArrayList<VkSemaphore> m_submitSemaphores;
	
	VkExtent2D m_extent;
	VkPresentModeKHR m_presentMode;
    VkSurfaceFormatKHR m_format;
};

}

#endif
