#ifndef KN_VULKAN_CONTEXT_HPP
#define KN_VULKAN_CONTEXT_HPP

#include "kon/container/array.hpp"
#include "kon/core/object.hpp"
#include "kon/core/reflection.hpp"
#include "kon/core/util.hpp"
#include "kon/engine/engine.hpp"
#include "kon/math/vector3.hpp"
#include "modules/graphics/vulkan/pipelines/vulkan_compute_screen_pipeline.hpp"
#include "modules/graphics/vulkan/pipelines/vulkan_mesh_pipeline.hpp"
#include "modules/graphics/vulkan/vulkan_cmd.hpp"
#include "modules/graphics/vulkan/vulkan_descriptors.hpp"
#include "modules/graphics/vulkan/vulkan_image.hpp"
#include "modules/graphics/vulkan/vulkan_pipeline.hpp"
#include "modules/graphics/vulkan/vulkan_swapchain.hpp"
#include <kon/debug/log.hpp>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <vk_mem_alloc.h>

namespace kon {

class VulkanMeshBuffer;

#define KN_ENABLE_VALIDATION true

#define KN_VULKAN_ERR_CHECK(opp) if(opp != VK_SUCCESS) { KN_ERROR("VULKAN ERROR YOUR COOKED"); }

struct VulkanFrameData {
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
	void recreate_swapchain();
	void viewport_render_image(u32 width, u32 height);

public:
	void draw_clear(Color color);

public:
	inline VkDevice get_device() const { return m_device; }
	inline VkSurfaceKHR get_surface() const { return m_surface; }
	inline VkPhysicalDevice get_physical_device() const { return m_physicalDevice; }
	inline VulkanFrameData &get_framedata() { return m_frames[m_frameNumber]; }
	inline VmaAllocator get_vma_allocator() const { return m_vmaAllocator; }
	inline DescriptorAllocator &get_descriptor_allocator() { return m_globalDescriptorAllocator; }
	inline VkImageView get_render_image_view() const { return m_renderImageView.get_handle(); }
	inline VulkanSwapchain &get_swapchain() { return m_swapchain; }
	inline VkInstance get_instance() const { return m_instance; }
	inline VkFormat get_renderimage_format() const { return m_renderImage.get_format(); }
	inline VkQueue get_graphics_queue() const { return m_graphicsQueue; }
	inline VulkanImage get_depth_image() const { return m_depthImage; }

public:
	VkCommandBuffer start_singetime_commands();
	void end_singletime_commands(VkCommandBuffer cmd);

private:
	void create_instance();
	void create_debug_utils();
	void create_surface();
	void select_physical_device();
	void create_device();
	void create_allocator();
	void create_render_image();
	void create_frames();
	void create_descriptors();

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
	VmaAllocator m_vmaAllocator;
	VkSampleCountFlagBits m_msaaSamples;

	Array<VulkanFrameData, FRAME_OVERLAP> m_frames;

	VulkanImage m_renderImage;
	VulkanImageView m_renderImageView;
	VulkanImage m_depthImage;
	VulkanImageView m_depthImageView;

	DescriptorAllocator m_globalDescriptorAllocator;

	VulkanComputePipelineScreen m_computePipelineScreen;
	VulkanMeshPipeline m_meshPipeline;

	VulkanMeshBuffer *m_mesh;

	bool m_resizeRequested {false};

	u8 m_frameNumber {0};

public:
	VulkanComputePipelineScreen::PushConstant m_cpsPushConstants;

	Vector3 position;
	Vector3 rotation;
	Vector3 scale {1.0f, 1.0f, 1.0f};
	float cameraScale {1.22173};
	int renderMode{0};
};

/*
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Winvalid-offsetof"

KN_REFLECT(VulkanContext,
KN_REF_BEGIN_TYPES(
	KN_REF_TYPE(m_cpsPushConstants, true)
),
KN_REF_NO_FUNCTIONS);

#pragma GCC diagnostic pop
*/

}

#endif
