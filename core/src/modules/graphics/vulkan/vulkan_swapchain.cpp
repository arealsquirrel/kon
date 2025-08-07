
#include "vulkan_swapchain.hpp"
#include "kon/container/arraylist.hpp"
#include "kon/core/core.hpp"
#include "kon/debug/log.hpp"
#include "modules/graphics/vulkan/vulkan_util.hpp"
#include <algorithm>
#include <modules/graphics/vulkan/vulkan_context.hpp>
#include <vulkan/vulkan_core.h>

namespace kon {

VulkanSwapchain::VulkanSwapchain(Allocator *allocator, VulkanContext *context)
	: m_allocator(allocator), m_context(context),
	  m_images(allocator), m_views(allocator), m_submitSemaphores(allocator) {}

VulkanSwapchain::~VulkanSwapchain() = default;

void VulkanSwapchain::create(u32 width, u32 height) {
	KN_INFO("Querying swapchain support");
	VkPhysicalDevice device = m_context->get_physical_device();
	VkSurfaceKHR surface = m_context->get_surface();

	VkSurfaceCapabilitiesKHR capabilities;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &capabilities);

	ArrayList<VkSurfaceFormatKHR> formats(m_allocator);
	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
	if (formatCount != 0) {
	    formats.resize(formatCount);
		formats.set_count(formatCount);
	    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, formats.get_buffer());
	}

	ArrayList<VkPresentModeKHR> presentModes(m_allocator);
	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
	if (presentModeCount != 0) {
	    presentModes.resize(presentModeCount);
		presentModes.set_count(presentModeCount);
	    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, presentModes.get_buffer());
	}

	m_format = formats[0];
	formats.for_each([&](VkSurfaceFormatKHR &format) {
		if (format.format == VK_FORMAT_B8G8R8A8_UNORM && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			m_format = format;
		}
	});

	m_presentMode = VK_PRESENT_MODE_FIFO_KHR;
	presentModes.for_each([&](VkPresentModeKHR mode){
		if(mode == VK_PRESENT_MODE_FIFO_KHR) {
			m_presentMode = mode;
		}
	});

	m_extent.width = std::clamp(width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
	m_extent.height = std::clamp(height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

	KN_INFO("Creating swapchain");

	uint32_t imageCount = capabilities.minImageCount + 1;
	if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount) {
    	imageCount = capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = m_context->get_surface();
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = m_format.format;
	createInfo.imageColorSpace = m_format.colorSpace;
	createInfo.imageExtent = m_extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;

	
	QueueFamilyIndices indices = find_queue_families(device, surface);
	uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

	if (indices.graphicsFamily != indices.presentFamily) {
	    createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
	    createInfo.queueFamilyIndexCount = 2;
	    createInfo.pQueueFamilyIndices = queueFamilyIndices;
	} else {
	    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	    createInfo.queueFamilyIndexCount = 0; // Optional
	    createInfo.pQueueFamilyIndices = nullptr; // Optional
	}

	createInfo.preTransform = capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = m_presentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	KN_VULKAN_ERR_CHECK(vkCreateSwapchainKHR(m_context->get_device(), &createInfo, nullptr, &m_swapchain));
	
	vkGetSwapchainImagesKHR(m_context->get_device(), m_swapchain, &imageCount, nullptr);
	m_images.resize(imageCount);
	m_images.set_count(imageCount);
	vkGetSwapchainImagesKHR(m_context->get_device(), m_swapchain, &imageCount, m_images.get_buffer());

	m_views.resize(imageCount);
	m_views.set_count(imageCount);
	for (u32 i = 0; i < m_images.get_capacity(); i++) {
		VkImageViewCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = m_images[i];
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = m_format.format;
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		KN_VULKAN_ERR_CHECK(vkCreateImageView(m_context->get_device(), &createInfo, nullptr, &m_views.get_buffer()[i]));
	}

	VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphoreInfo.pNext = nullptr;
	m_submitSemaphores.resize(imageCount);
	m_submitSemaphores.set_count(imageCount);
	for (u32 i = 0; i < m_submitSemaphores.get_capacity(); i++) {
		KN_VULKAN_ERR_CHECK(
				vkCreateSemaphore(m_context->get_device(), &semaphoreInfo,
					nullptr, &m_submitSemaphores[i]));
	}
}

u32 VulkanSwapchain::aquire_next_image(VkSemaphore s) {
	u32 swapchainImageIndex {};
	KN_VULKAN_ERR_CHECK(
			vkAcquireNextImageKHR(m_context->get_device(),
				m_swapchain, 1000000000, s, nullptr, &swapchainImageIndex));
	return swapchainImageIndex;
}

void VulkanSwapchain::destroy() {
	for(u32 i = 0; i < m_views.get_capacity(); i++) {
		vkDestroyImageView(m_context->get_device(), m_views.get(i), nullptr);
		vkDestroySemaphore(m_context->get_device(), m_submitSemaphores.get(i), nullptr);
	}

	vkDestroySwapchainKHR(m_context->get_device(), m_swapchain, nullptr);
}

}

