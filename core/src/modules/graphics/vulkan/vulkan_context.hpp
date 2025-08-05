#ifndef KN_VULKAN_CONTEXT_HPP
#define KN_VULKAN_CONTEXT_HPP

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

namespace kon {

/*
 * all vulkan objects will contain the vulkan context
 */
class VulkanContext {
public:
	VulkanContext();
	~VulkanContext();

	// creates all the fun vulkan things
	void init_vulkan();

	// cleans all the vulkan things
	void clean_vulkan();

private:
	void create_instance();
	void create_debug_utils();
	void select_physical_device();
	void create_device();
	void create_swapchain();

private:
	VkInstance m_instance;
	VkPhysicalDevice m_physicalDevice;
	VkDevice m_device;
	VkDebugUtilsMessengerEXT _debug_messenger;
};

}

#endif
