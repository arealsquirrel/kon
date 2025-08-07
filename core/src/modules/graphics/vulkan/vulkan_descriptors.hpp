#ifndef KN_VULKAN_DESCRIPTORS
#define KN_VULKAN_DESCRIPTORS

#include "kon/container/arraylist.hpp"
#include "kon/core/allocator.hpp"
#include <vulkan/vulkan_core.h>

namespace kon {

struct DescriptorLayoutBuilder {
    ArrayList<VkDescriptorSetLayoutBinding> bindings;

public:
    void add_binding(uint32_t binding, VkDescriptorType type);
    void clear() { bindings.reset(); }

    VkDescriptorSetLayout build(VkDevice device, VkShaderStageFlags shaderStages, void* pNext = nullptr, VkDescriptorSetLayoutCreateFlags flags = 0);
};

struct DescriptorAllocator {
    struct PoolSizeRatio{
		VkDescriptorType type;
		float ratio;
    };

    VkDescriptorPool pool;

    void init_pool(Allocator *allocator, VkDevice device, uint32_t maxSets, std::initializer_list<PoolSizeRatio> poolRatios);
    void clear_descriptors(VkDevice device);
    void destroy_pool(VkDevice device);

    VkDescriptorSet allocate(VkDevice device, VkDescriptorSetLayout layout);
};

}

#endif
