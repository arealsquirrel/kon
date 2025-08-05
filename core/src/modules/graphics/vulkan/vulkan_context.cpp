
#include "vulkan_context.hpp"
#include <kon/debug/log.hpp>

namespace kon {

VulkanContext::VulkanContext() {

}

VulkanContext::~VulkanContext() {

}

void VulkanContext::init_vulkan() {
	create_instance();
	select_physical_device();
	create_device();
}

void VulkanContext::clean_vulkan() {

}

void VulkanContext::create_instance() {
	KN_INFO("creating vulkan instance");
	// turn on validation and instance extensions
	

}

void VulkanContext::create_debug_utils() {

}

void VulkanContext::select_physical_device() {
	KN_INFO("selecting physical device");
}

void VulkanContext::create_device() {
	KN_INFO("creating logical device");
}

void VulkanContext::create_swapchain() {
	KN_INFO("creating swapchain");
}

}

