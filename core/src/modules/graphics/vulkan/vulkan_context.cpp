
#include "vulkan_context.hpp"
#include "kon/core/object.hpp"
#include "modules/graphics/vulkan/vulkan_util.hpp"
#include <kon/debug/log.hpp>
#include <set>
#include <vulkan/vulkan_core.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace kon {

VulkanContext::VulkanContext(Engine *engine) 
	: Object(engine),
	m_swapchain(engine->get_allocator_dynamic(), this),
	m_commandPool(engine->get_allocator_dynamic(), this),
	m_acquireSemaphore(engine->get_allocator_dynamic()),
	m_inFlightFence(engine->get_allocator_dynamic()),
	m_submitSemaphores(engine->get_allocator_dynamic()) {}

VulkanContext::~VulkanContext() {

}

void VulkanContext::init_vulkan() {
	create_instance();
	create_surface();
	select_physical_device();
	create_device();
	m_swapchain.create(0,0);
	create_frames();
}

void VulkanContext::clean_vulkan() {
	vkDeviceWaitIdle(m_device);

	for (u32 i = 0; i < FRAME_OVERLAP; i++) {
		vkDestroySemaphore(m_device, m_acquireSemaphore[i], nullptr);
		vkDestroyFence(m_device, m_inFlightFence[i], nullptr);
	}

	for(u32 i = 0; i < m_swapchain.get_image_array().get_capacity(); i++)
		vkDestroySemaphore(m_device, m_submitSemaphores[i], nullptr);

	m_commandPool.destroy();
	m_swapchain.destroy();

	vkDestroyDevice(m_device, nullptr);

	if(KN_ENABLE_VALIDATION) {
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(m_instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr) {
			func(m_instance, m_debugMessenger, nullptr);
		}
	}

	vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
	vkDestroyInstance(m_instance, nullptr);
}

void VulkanContext::create_instance() {
	KN_INFO("creating vulkan instance");
	
	uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	ArrayList<const char *> extensions(m_engine->get_allocator_frame(), glfwExtensionCount+1);
	for(u32 i = 0; i < glfwExtensionCount; i++)
		extensions.add(glfwExtensions[i]);

	if (KN_ENABLE_VALIDATION) {
        extensions.add(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

	VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "rhaaa";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "kon";
    appInfo.engineVersion = VK_MAKE_VERSION(KN_VERSION_MAJOR, KN_VERSION_MINOR, KN_VERSION_PATCH);
    appInfo.apiVersion = VK_API_VERSION_1_3;

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;
	createInfo.enabledExtensionCount = static_cast<uint32_t>(glfwExtensionCount+1);
	createInfo.ppEnabledExtensionNames = extensions.get_buffer();

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
	if (KN_ENABLE_VALIDATION) {
		KN_INFO("enabling validation");
    	createInfo.enabledLayerCount = static_cast<uint32_t>(validation_layers.get_count());
    	createInfo.ppEnabledLayerNames = validation_layers.get_buffer();
		
		debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		debugCreateInfo.pfnUserCallback = debug_callback;

        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
	} else {
    	createInfo.enabledLayerCount = 0;
		createInfo.pNext = nullptr;
	}

	KN_VULKAN_ERR_CHECK(vkCreateInstance(&createInfo, nullptr, &m_instance));

	if(KN_ENABLE_VALIDATION) {
		VkDebugUtilsMessengerCreateInfoEXT debugInfo {};
		debugInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debugInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		debugInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		debugInfo.pfnUserCallback = debug_callback;
	
	    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(m_instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr) {
			func(m_instance, &debugInfo, nullptr, &m_debugMessenger);
		} else {
			KN_WARN("Instance extension for debug logging not present");
		}
	}
}

void VulkanContext::create_surface() {
	KN_INFO("Getting glfw surface from engine window");
	KN_VULKAN_ERR_CHECK(glfwCreateWindowSurface(m_instance, m_engine->get_window().get_handle(), nullptr, &m_surface));
}

void VulkanContext::select_physical_device() {
	KN_INFO("selecting physical device");
	u32 deviceCount = 0;
	vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);

	if(deviceCount == 0) {
		KN_ERROR("Vulkan cant find any GPU's for it to use :(");
	}

	ArrayList<VkPhysicalDevice> devices(m_engine->get_allocator_frame(), deviceCount);
	vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.get_buffer());
	
	for(u32 i = 0; i < devices.get_capacity(); i++) {
		if(is_device_suitable(devices[i], m_surface, m_engine->get_allocator_frame())) {
			m_physicalDevice = devices[i];
		}
	}
	
	vkGetPhysicalDeviceProperties(m_physicalDevice, &m_deviceProperties);
	m_msaaSamples = find_maximum_samples(m_deviceProperties);

	if(m_physicalDevice == VK_NULL_HANDLE) {
		KN_ERROR("we cant find a suitable gpu :(");
	}
}

void VulkanContext::create_device() {
	KN_INFO("creating logical device");

	QueueFamilyIndices indices = find_queue_families(m_physicalDevice, m_surface);

	// ! TODO STANDARD LIBRARY IS FUCKING EVIIILLL
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<u32> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies) {
    	VkDeviceQueueCreateInfo queueCreateInfo{};
    	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    	queueCreateInfo.queueFamilyIndex = queueFamily;
   		queueCreateInfo.queueCount = 1;
    	queueCreateInfo.pQueuePriorities = &queuePriority;
    	queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceSynchronization2Features sync2 {};
	sync2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SYNCHRONIZATION_2_FEATURES;
	sync2.synchronization2 = true;

	VkPhysicalDeviceFeatures2 deviceFeatures {};
	deviceFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
	deviceFeatures.pNext = &sync2;
	deviceFeatures.features.samplerAnisotropy = VK_TRUE;

	vkGetPhysicalDeviceFeatures2(m_physicalDevice, &deviceFeatures);

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.queueCreateInfoCount = static_cast<u32>(queueCreateInfos.size());
	createInfo.pNext = &deviceFeatures;
	//createInfo.pEnabledFeatures = &deviceFeatures;

	createInfo.enabledExtensionCount = static_cast<u32>(device_extentions.get_capacity());
	createInfo.ppEnabledExtensionNames = device_extentions.get_buffer();

	if (KN_ENABLE_VALIDATION) {
    	createInfo.enabledLayerCount = static_cast<uint32_t>(validation_layers.get_capacity());
    	createInfo.ppEnabledLayerNames = validation_layers.get_buffer();
	} else {
    	createInfo.enabledLayerCount = 0;
	}

	KN_VULKAN_ERR_CHECK(vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_device));
	vkGetDeviceQueue(m_device, indices.graphicsFamily.value(), 0, &m_graphicsQueue);
	vkGetDeviceQueue(m_device, indices.presentFamily.value(), 0, &m_presentQueue);

	KN_INFO("Logical Device Created.");
	KN_INFO("{} with {} samples", m_deviceProperties.deviceName, (u32)m_msaaSamples);

	KN_INFO("Creating command pool");
	m_commandPool.create(indices.graphicsFamily.value(), FRAME_OVERLAP);
}

void VulkanContext::create_frames() {
	KN_INFO("Creating frames");
	m_acquireSemaphore.resize(m_swapchain.get_image_array().get_capacity());
	m_inFlightFence.resize(FRAME_OVERLAP);
	m_submitSemaphores.resize(FRAME_OVERLAP);

	for(u32 i = 0; i < FRAME_OVERLAP; i++) {
		VkSemaphoreCreateInfo sInfo = vkutil::semaphore_create_info(0);
		KN_VULKAN_ERR_CHECK(vkCreateSemaphore(m_device, &sInfo, nullptr, &m_acquireSemaphore[i]));
	
		VkFenceCreateInfo fInfo = vkutil::fence_create_info(VK_FENCE_CREATE_SIGNALED_BIT);
		KN_VULKAN_ERR_CHECK(vkCreateFence(m_device, &fInfo, nullptr, &m_inFlightFence[i]));
	}

	for(u32 i = 0; i < m_swapchain.get_image_array().get_capacity(); i++) {
		VkSemaphoreCreateInfo sInfo = vkutil::semaphore_create_info(0);
		KN_VULKAN_ERR_CHECK(vkCreateSemaphore(m_device, &sInfo, nullptr, &m_submitSemaphores[i]));
	}
}

void VulkanContext::start_frame() {
	KN_VULKAN_ERR_CHECK(vkWaitForFences(m_device, 1, &m_inFlightFence[m_frameNumber], true, 1000000000));
	KN_VULKAN_ERR_CHECK(vkResetFences(m_device, 1, &m_inFlightFence[m_frameNumber]));

	vkAcquireNextImageKHR(m_device, m_swapchain.get_swapchain(),
			1000000000,
			m_acquireSemaphore[m_frameNumber], nullptr, &m_currentSwapchainIndex);

	VkCommandBuffer cmd = m_commandPool.get_buffer(m_frameNumber);
	vkResetCommandBuffer(cmd, 0);
	VkCommandBufferBeginInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    info.pNext = nullptr;
    info.pInheritanceInfo = nullptr;
    info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	vkBeginCommandBuffer(cmd, &info);

	vkutil::transition_image(cmd, m_swapchain.get_image(m_currentSwapchainIndex), VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_GENERAL);
}

void VulkanContext::end_frame() {
	vkutil::transition_image(m_commandPool.get_buffer(m_frameNumber), m_swapchain.get_image(m_currentSwapchainIndex),
			VK_IMAGE_LAYOUT_GENERAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);

	vkEndCommandBuffer(m_commandPool.get_buffer(m_frameNumber));
}

void VulkanContext::present() {
	VkCommandBufferSubmitInfo cmdinfo = vkutil::command_buffer_submit_info(m_commandPool.get_buffer(m_frameNumber));	
	
	VkSemaphoreSubmitInfo waitInfo = vkutil::semaphore_submit_info(
			VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT_KHR,
			m_acquireSemaphore[m_frameNumber]);

	VkSemaphoreSubmitInfo signalInfo = vkutil::semaphore_submit_info(
			VK_PIPELINE_STAGE_2_ALL_GRAPHICS_BIT,
			m_submitSemaphores[m_currentSwapchainIndex]);	
	
	VkSubmitInfo2 submit = vkutil::submit_info(&cmdinfo,&signalInfo,&waitInfo);	

	//submit command buffer to the queue and execute it.
	// _renderFence will now block until the graphic commands finish execution
	KN_VULKAN_ERR_CHECK(vkQueueSubmit2(m_graphicsQueue, 1, &submit, m_inFlightFence[m_frameNumber]));

	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.pNext = nullptr;
	VkSwapchainKHR swp = m_swapchain.get_swapchain();
	presentInfo.pSwapchains = &swp;
	presentInfo.swapchainCount = 1;
	presentInfo.pWaitSemaphores = &m_submitSemaphores[m_currentSwapchainIndex];
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pImageIndices = &m_currentSwapchainIndex;

	KN_VULKAN_ERR_CHECK(vkQueuePresentKHR(m_graphicsQueue, &presentInfo));

	//increase the number of frames drawn
	m_frameNumber = (m_frameNumber+1) % FRAME_OVERLAP;
}

void VulkanContext::draw_clear(Color color) {
	VkClearColorValue clearValue {{color.r, color.g, color.b, color.a}};
	// float flash = std::abs(std::sin(_frameNumber / 120.f));
	// clearValue = { { 0.0f, 0.0f, flash, 1.0f } };

	VkImageSubresourceRange clearRange = vkutil::image_subresource_range(VK_IMAGE_ASPECT_COLOR_BIT);

	//clear image
	vkCmdClearColorImage(m_commandPool.get_buffer(m_frameNumber),
			m_swapchain.get_image(m_currentSwapchainIndex),
			VK_IMAGE_LAYOUT_GENERAL, &clearValue, 1, &clearRange);

}

}

