#ifndef KN_SWAPCHAIN_HPP
#define KN_SWAPCHAIN_HPP

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

private:
	Allocator *m_allocator;
	VulkanContext *m_context;

	VkSwapchainKHR m_swapchain;
	ArrayList<VkImage> m_images;
	ArrayList<VkImageView> m_views;
	
	VkExtent2D m_extent;
	VkPresentModeKHR m_presentMode;
    VkSurfaceFormatKHR m_format;
};

}

#endif
