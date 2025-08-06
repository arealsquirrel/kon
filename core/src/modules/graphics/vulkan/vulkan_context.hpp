#ifndef KN_VULKAN_CONTEXT_HPP
#define KN_VULKAN_CONTEXT_HPP

#include "kon/container/array.hpp"
#include "kon/container/arraylist.hpp"
#include "kon/core/object.hpp"
#include "kon/core/util.hpp"
#include "kon/engine/engine.hpp"
#include "modules/graphics/vulkan/vulkan_cmd.hpp"
#include "modules/graphics/vulkan/vulkan_swapchain.hpp"
#include <kon/debug/log.hpp>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

namespace kon {

#define KN_ENABLE_VALIDATION true

#define KN_VULKAN_ERR_CHECK(opp) if(opp != VK_SUCCESS) { KN_ERROR("VULKAN ERROR YOUR COOKED"); }

struct VulkanFrameData {
	VkCommandBuffer cmd;
	VkSemaphore acquireSemaphore;
	VkFence presentFence;
	u32 swapchainIndex;
};

/*
 * all vulkan objects will contain the vulkan context
 */
class VulkanContext : public Object {
KN_OBJECT(VulkanContext, Object)

public:
	static constexpr u32 FRAME_OVERLAP = 2;

public:
	VulkanContext(Engine *engine);
	~VulkanContext();

	// creates all the fun vulkan things
	void init_vulkan();

	// cleans all the vulkan things
	void clean_vulkan();

	// opens up the command buffer for drawing
	void start_frame();

	// closes the command buffer
	void end_frame();

	// calls vkpresent and vkwhateverthefuck to render all your crap
	void present();

public:
	void draw_clear(Color color);

public:
	inline VkDevice get_device() const { return m_device; }
	inline VkSurfaceKHR get_surface() const { return m_surface; }
	inline VkPhysicalDevice get_physical_device() const { return m_physicalDevice; }
	inline VulkanFrameData &get_framedata() { return m_frames[m_frameNumber]; }

private:
	void create_instance();
	void create_debug_utils();
	void create_surface();
	void select_physical_device();
	void create_device();
	void create_frames();

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
	VulkanCommandPool m_commandPool;
	VkSampleCountFlagBits m_msaaSamples;

	Array<VulkanFrameData, FRAME_OVERLAP> m_frames;

	u8 m_frameNumber {0};
};

}

#endif
