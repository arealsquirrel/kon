
#include "vulkan_shader.hpp"
#include "modules/graphics/vulkan/vulkan_context.hpp"
#include <vulkan/vulkan_core.h>

namespace kon {

VulkanShader::VulkanShader(VulkanContext *context) 
	: m_context(context) {}

VulkanShader::~VulkanShader() = default;

void VulkanShader::create(const char *shader, u32 size) {
	VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.pNext = nullptr;

    // codeSize has to be in bytes, so multply the ints in the buffer by size of
    // int to know the real size of the buffer
    createInfo.codeSize = size;
    createInfo.pCode = reinterpret_cast<const u32*>(shader);

    KN_VULKAN_ERR_CHECK(vkCreateShaderModule(m_context->get_device(), &createInfo, nullptr, &m_module));
}

void VulkanShader::destroy() {
	vkDestroyShaderModule(m_context->get_device(), m_module, nullptr);
}

}

