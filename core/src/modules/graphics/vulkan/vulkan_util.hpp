#ifndef KN_VULKAN_UTIL_HPP
#define KN_VULKAN_UTIL_HPP

#include "kon/container/array.hpp"
#include "kon/core/allocator.hpp"
#include "kon/debug/log.hpp"
#include <optional>
#include <vulkan/vulkan_core.h>

namespace kon {

const Array<const char*, 1> validation_layers = {
	"VK_LAYER_KHRONOS_validation",
};

const Array<const char*, 5> device_extentions = {
	// lets us use the swapchain
	VK_KHR_SWAPCHAIN_EXTENSION_NAME,

	// NO MORE RENDERPASSES AND LIKE CRAP AND SHIT
	VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME,

	// makes sync not suck
	VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME,

	// adds “pointers to buffer memory in shaders”.
	VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME,

	// you can make one bigboy buffer with all your textures and just use that
	VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
	
};

struct QueueFamilyIndices {
	std::optional<u32> graphicsFamily;
	std::optional<u32> presentFamily;

	bool is_complete() {
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

QueueFamilyIndices find_queue_families(
		VkPhysicalDevice device, VkSurfaceKHR surface);

inline VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {
	
	if(messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
		KN_WARN("Validation layer: {}", pCallbackData->pMessage);
	} else if(messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
		KN_ERROR("Validation layer: {}", pCallbackData->pMessage);
	} else {
		KN_TRACE("Validation layer: {}", pCallbackData->pMessage);
	}

    return VK_FALSE;
}

bool is_device_suitable(VkPhysicalDevice device, VkSurfaceKHR surface, Allocator *allocator);

VkSampleCountFlagBits find_maximum_samples(VkPhysicalDeviceProperties props);

}

#endif
