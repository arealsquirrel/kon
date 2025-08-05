
#include "vulkan_util.hpp"
#include "kon/container/arraylist.hpp"
#include "kon/debug/log.hpp"
#include <cstring>
#include <vulkan/vulkan_core.h>

namespace kon {

QueueFamilyIndices find_queue_families(VkPhysicalDevice device, VkSurfaceKHR surface) {
	QueueFamilyIndices indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies) {
    	if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
        	indices.graphicsFamily = i;
    	}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
		if(presentSupport) {
			indices.presentFamily = i;
		}

		if(indices.is_complete()) {
			break;
		}

    	i++;
	}

	return indices;
}

SwapChainSupportDetails get_swap_chain_support_details(VkPhysicalDevice device, VkSurfaceKHR surface) {
	SwapChainSupportDetails details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

	/*
	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
	if (formatCount != 0) {
	    details.formats.resize(formatCount);
	    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
	if (presentModeCount != 0) {
	    details.presentModes.resize(presentModeCount);
	    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
	}
	*/

	return details;
}

bool is_device_suitable(VkPhysicalDevice device, VkSurfaceKHR surface, Allocator *allocator) {
	VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

	KN_TRACE("checking device {}", deviceProperties.deviceName);

	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
	ArrayList<VkExtensionProperties> availableExtensions(allocator, extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.get_buffer());
	u32 foundCount = 0;
	for(u32 i = 0; i < availableExtensions.get_capacity(); i++) {
		auto extension = availableExtensions[i];

		for(u32 j = 0; j < device_extentions.get_capacity(); j++) {
			if(strcmp(extension.extensionName, device_extentions[j]) == 0) {
				foundCount += 1;

				KN_TRACE("[{}/{}] Found required extension {}", foundCount, device_extentions.get_count(), extension.extensionName);
			}
		}
	}

	bool extensionsSupported = (foundCount == device_extentions.get_count());

	if(!extensionsSupported) {
		KN_ERROR("\t device does not support needed extensions :( ");
		return false;
	}

	bool swapChainAdequate = false;
	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

	swapChainAdequate = (formatCount != 0 && presentModeCount != 0);

	if(!swapChainAdequate)
		KN_WARN("\t swap chain is not adequate");

	QueueFamilyIndices indices = find_queue_families(device, surface);
	return indices.is_complete() && extensionsSupported && swapChainAdequate && deviceFeatures.samplerAnisotropy;
}

VkSampleCountFlagBits find_maximum_samples(VkPhysicalDeviceProperties props) {
    VkSampleCountFlags counts = props.limits.framebufferColorSampleCounts & props.limits.framebufferDepthSampleCounts;
    if (counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
    if (counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
    if (counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
    if (counts & VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
    if (counts & VK_SAMPLE_COUNT_4_BIT) { return VK_SAMPLE_COUNT_4_BIT; }
    if (counts & VK_SAMPLE_COUNT_2_BIT) { return VK_SAMPLE_COUNT_2_BIT; }
    return VK_SAMPLE_COUNT_1_BIT;
}

}

