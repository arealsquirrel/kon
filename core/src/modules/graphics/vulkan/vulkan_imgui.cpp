
#include "vulkan_imgui.hpp"
#include "kon/math/vector4.hpp"
#include "modules/graphics/vulkan/vulkan_util.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>
#include <vulkan/vulkan_core.h>

namespace kon {

VulkanImgui::ColorScheme make_gruvbox_colorscheme() {
	auto hex_to_imgvec = [](int r, int g, int b){ return ImVec4{r/256.0f, g/256.0f, b/256.0f, 1.0}; };

	VulkanImgui::ColorScheme scheme;

	scheme.bg0 = hex_to_imgvec(40, 40, 40);
	scheme.bg0_h = hex_to_imgvec(29, 32, 33);
	scheme.bg1 = hex_to_imgvec(60, 56, 54);
	scheme.red = hex_to_imgvec(204, 36, 29);
	scheme.red1 = hex_to_imgvec(251,73,52);
	scheme.green = hex_to_imgvec(152, 151, 26);
	scheme.green1 = hex_to_imgvec(184,187,38);
	scheme.yellow = hex_to_imgvec(215, 153, 33);
	scheme.yellow1 = hex_to_imgvec(250,189,47);
	scheme.blue = hex_to_imgvec(69, 133, 136);
	scheme.blue1 = hex_to_imgvec(131,165,152);
	scheme.gray = hex_to_imgvec(168, 153, 132);
	scheme.fg = hex_to_imgvec(235, 219, 178);

	return scheme;
}

void style(ImGuiIO &) {
	VulkanImgui::ColorScheme colors = make_gruvbox_colorscheme();

	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowMinSize        = ImVec2( 160, 20 );
    style.FramePadding         = ImVec2( 10, 4 );
    style.ItemSpacing          = ImVec2( 8, 6 );
    style.ItemInnerSpacing     = ImVec2( 7, 3 );
    style.Alpha                = 1.0f;
    style.ItemInnerSpacing     = ImVec2( 2, 4 );
    style.WindowRounding       = 10.0f;
    style.FrameRounding        = 12.0f;
    style.GrabMinSize          = 14.0f;
    style.GrabRounding         = 16.0f;
    style.ScrollbarSize        = 12.0f;
    style.ScrollbarRounding    = 16.0f;
	style.ChildRounding = 6.0f;
	style.PopupRounding = 6.0f;

	style.TreeLinesFlags 	   = ImGuiTreeNodeFlags_DrawLinesToNodes;
	style.TreeLinesSize 	   = 1.0f;

	style.Colors[ImGuiCol_WindowBg] = colors.bg0;
	style.Colors[ImGuiCol_TitleBg] = colors.bg0;
	style.Colors[ImGuiCol_TitleBgActive] = colors.bg0_h;
	style.Colors[ImGuiCol_TitleBgCollapsed] = colors.bg0;
	style.Colors[ImGuiCol_Text]              	 = colors.fg;
    style.Colors[ImGuiCol_TextDisabled]          = colors.gray;
    style.Colors[ImGuiCol_Border]                = colors.bg0_h;
    style.Colors[ImGuiCol_BorderShadow]          = colors.bg0_h;

	style.Colors[ImGuiCol_FrameBg]               = colors.bg1;
    style.Colors[ImGuiCol_FrameBgHovered]        = colors.bg1;
    style.Colors[ImGuiCol_FrameBgActive]         = colors.bg1;

	style.Colors[ImGuiCol_MenuBarBg]             = colors.bg1;
    style.Colors[ImGuiCol_ScrollbarBg]           = colors.bg0;
    style.Colors[ImGuiCol_ScrollbarGrab]         = colors.bg1;
    style.Colors[ImGuiCol_ScrollbarGrabHovered]  = colors.bg1;
    style.Colors[ImGuiCol_ScrollbarGrabActive]   = colors.bg1;

	style.Colors[ImGuiCol_CheckMark]             = colors.green;
    style.Colors[ImGuiCol_SliderGrab]            = colors.green;
    style.Colors[ImGuiCol_SliderGrabActive]      = colors.green1;
    style.Colors[ImGuiCol_Button]                = colors.green;
    style.Colors[ImGuiCol_ButtonHovered]         = colors.green;
    style.Colors[ImGuiCol_ButtonActive]          = colors.green1;
	
	style.Colors[ImGuiCol_DockingPreview] 		 = colors.green;
	style.Colors[ImGuiCol_DockingEmptyBg] 		 = colors.green1;

	style.Colors[ImGuiCol_Tab] 					 = colors.bg0;
	style.Colors[ImGuiCol_TabHovered] 			 = colors.bg1;
	style.Colors[ImGuiCol_TabSelected] 			 = colors.bg1;
	style.Colors[ImGuiCol_TabSelectedOverline] 	 = colors.bg0;
	style.Colors[ImGuiCol_TabDimmed] 			 = colors.bg0;
	style.Colors[ImGuiCol_TabDimmedSelected] 	 = colors.bg1;
	style.Colors[ImGuiCol_TabDimmedSelectedOverline] = colors.bg1;

	style.Colors[ImGuiCol_Header]                = colors.bg1;
    style.Colors[ImGuiCol_HeaderHovered]         = colors.gray;
    style.Colors[ImGuiCol_HeaderActive]          = colors.gray;

	style.Colors[ImGuiCol_Separator]             = colors.bg0_h;
    style.Colors[ImGuiCol_SeparatorHovered]      = colors.bg0_h;
    style.Colors[ImGuiCol_SeparatorActive]       = colors.bg0_h;
    style.Colors[ImGuiCol_ResizeGrip]            = colors.blue;
    style.Colors[ImGuiCol_ResizeGripHovered]     = colors.blue1;
    style.Colors[ImGuiCol_ResizeGripActive]      = colors.blue1;
    style.Colors[ImGuiCol_PlotLines]             = colors.yellow;
    style.Colors[ImGuiCol_PlotLinesHovered]      = colors.yellow1;
    style.Colors[ImGuiCol_PlotHistogram]         = colors.yellow;
    style.Colors[ImGuiCol_PlotHistogramHovered]  = colors.yellow1;
    style.Colors[ImGuiCol_PopupBg]               = colors.bg0;
}

void VulkanImgui::init(VulkanContext *context, GLFWwindow *window) {
	ImGui::CreateContext();
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	style(ImGui::GetIO());

	ImGui_ImplGlfw_InitForVulkan(window, true);

    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.ApiVersion = VK_API_VERSION_1_3;              // Pass in your value of VkApplicationInfo::apiVersion, otherwise will default to header version.
    init_info.Instance = context->get_instance();
    init_info.PhysicalDevice = context->get_physical_device();
    init_info.Device = context->get_device();
    init_info.QueueFamily = find_queue_families(context->get_physical_device(), context->get_surface()).graphicsFamily.value();
    init_info.Queue = context->get_graphics_queue();
    init_info.DescriptorPool = context->get_imgui_pool();
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

