#ifndef KN_VULKAN_SHADER
#define KN_VULKAN_SHADER

#include "kon/core/core.hpp"
#include <vulkan/vulkan_core.h>

namespace kon {

class VulkanContext;

class VulkanShader {
public:
	VulkanShader(VulkanContext *context);
	~VulkanShader();

	void create(const char *shader, u32 size);
	void destroy();

public:
	inline VkShaderModule get_shader() const { return m_module; }

private:
	VulkanContext *m_context;

	VkShaderModule m_module;
};

}

#endif
