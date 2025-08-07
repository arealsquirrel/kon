#ifndef KN_VULKAN_IMGUI_HPP
#define KN_VULKAN_IMGUI_HPP

#include "modules/graphics/vulkan/vulkan_context.hpp"

namespace kon {

class VulkanImgui {
public:
	static void init(VulkanContext *context, GLFWwindow *window);
	static void clean();
};

}

#endif
