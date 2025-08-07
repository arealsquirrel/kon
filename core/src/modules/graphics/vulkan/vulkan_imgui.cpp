
#include "vulkan_imgui.hpp"
#include "modules/graphics/vulkan/vulkan_util.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>
#include <vulkan/vulkan_core.h>

namespace kon {

void VulkanImgui::init(VulkanContext *context, GLFWwindow *window) {
	ImGui::CreateContext();
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_IsSRGB;

	ImGui_ImplGlfw_InitForVulkan(window, true);

    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.ApiVersion = VK_API_VERSION_1_3;              // Pass in your value of VkApplicationInfo::apiVersion, otherwise will default to header version.
    init_info.Instance = context->get_instance();
    init_info.PhysicalDevice = context->get_physical_device();
    init_info.Device = context->get_device();
    init_info.QueueFamily = find_queue_families(context->get_physical_device(), context->get_surface()).graphicsFamily.value();
    init_info.Queue = context->get_graphics_queue();
    init_info.DescriptorPool = context->get_descriptor_allocator().pool;
	init_info.UseDynamicRendering = true;
    init_info.MinImageCount = 3;
    init_info.ImageCount = 3;
    init_info.Allocator = nullptr;

	VkFormat format = context->get_swapchain().get_format().format;
	init_info.PipelineRenderingCreateInfo = {};
	init_info.PipelineRenderingCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
	init_info.PipelineRenderingCreateInfo.colorAttachmentCount = 1;
	init_info.PipelineRenderingCreateInfo.pColorAttachmentFormats = &format;
	init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

 	ImGui_ImplVulkan_Init(&init_info);
}

void VulkanImgui::clean() {
	ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

}

