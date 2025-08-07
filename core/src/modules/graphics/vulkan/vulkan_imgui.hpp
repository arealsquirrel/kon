#ifndef KN_VULKAN_IMGUI_HPP
#define KN_VULKAN_IMGUI_HPP

#include "modules/graphics/vulkan/vulkan_context.hpp"
#include <imgui.h>

namespace kon {

class VulkanImgui {
public:
	struct ColorScheme {
		ImVec4 bg0;
		ImVec4 bg0_h;
		ImVec4 bg1;
		ImVec4 red;
		ImVec4 red1;
		ImVec4 green;
		ImVec4 green1;
		ImVec4 yellow;
		ImVec4 yellow1;
		ImVec4 blue;
		ImVec4 blue1;
		ImVec4 gray;
		ImVec4 fg;
	};

public:
	static void init(VulkanContext *context, GLFWwindow *window);
	static void clean();
};

}

#endif
