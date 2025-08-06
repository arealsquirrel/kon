#ifndef KN_VULKAN_CONTEXT_HPP
#define KN_VULKAN_CONTEXT_HPP

#include "kon/core/object.hpp"
#include "kon/engine/engine.hpp"
#include "modules/graphics/vulkan/vulkan_swapchain.hpp"
#include <kon/debug/log.hpp>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

namespace kon {

#define KN_ENABLE_VALIDATION true

#define KN_VULKAN_ERR_CHECK(opp) if(opp != VK_SUCCESS) { KN_ERROR("VULKAN ERROR YOUR COOKED"); }

/*
 * all vulkan objects will contain the vulkan context
 */
class VulkanContext : public Object {
KN_OBJECT(VulkanContext, Object)

public:
	VulkanContext(Engine *engine);
	~VulkanContext();

	// creates all the fun vulkan things
	void init_vulkan();

	// cleans all the vulkan things
	void clean_vulkan();

public:
	inline VkDevice get_device() const { return m_device; }
	inline VkSurfaceKHR get_surface() const { return m_surface; }
	inline VkPhysicalDevice get_physical_device() const { return m_physicalDevice; }

private:
	void create_instance();
	void create_debug_utils();
	void create_surface();
	void select_physical_device();
	void create_device();

private:
	VkInstance m_instance;
	VkDebugUtilsMessengerEXT m_debugMessenger;
	VkSurfaceKHR m_surface;
	VkPhysicalDevice m_physicalDevice;
	VkPhysicalDeviceProperties m_deviceProperties;
	VkDevice m_device;

	VkQueue m_graphicsQueue;
	VkQueue m_presentQueue;

	VulkanSwapchain m_swapchain;

	VkSampleCountFlagBits m_msaaSamples;
};

}

#endif
