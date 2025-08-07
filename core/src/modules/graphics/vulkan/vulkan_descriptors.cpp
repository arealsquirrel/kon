
#include "vulkan_descriptors.hpp"
#include "kon/container/arraylist.hpp"
#include "modules/graphics/vulkan/vulkan_context.hpp"

namespace kon {

void DescriptorLayoutBuilder::add_binding(uint32_t binding, VkDescriptorType type) {
    VkDescriptorSetLayoutBinding newbind {};
    newbind.binding = binding;
    newbind.descriptorCount = 1;
    newbind.descriptorType = type;
    bindings.add(newbind);
}

VkDescriptorSetLayout DescriptorLayoutBuilder::build(VkDevice device,
		VkShaderStageFlags shaderStages, void* pNext,
		VkDescriptorSetLayoutCreateFlags flags) {

	bindings.for_each([&](auto &b){
        b.stageFlags |= shaderStages;
    });

    VkDescriptorSetLayoutCreateInfo info;
	info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    info.pNext = pNext;

    info.pBindings = bindings.get_buffer();
    info.bindingCount = (uint32_t)bindings.get_capacity();
    info.flags = flags;

    VkDescriptorSetLayout set;
    KN_VULKAN_ERR_CHECK(vkCreateDescriptorSetLayout(device, &info, nullptr, &set));

    return set;
}

void DescriptorAllocator::init_pool(Allocator *allocator, VkDevice device, uint32_t maxSets, std::initializer_list<PoolSizeRatio> poolRatios) {
	ArrayList<VkDescriptorPoolSize> poolSizes(allocator, poolRatios.size());
    for (PoolSizeRatio ratio : poolRatios) {
        poolSizes.add(VkDescriptorPoolSize{
            .type = ratio.type,
            .descriptorCount = uint32_t(ratio.ratio * maxSets)
        });
    }

	VkDescriptorPoolCreateInfo pool_info = {}; 
	pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	pool_info.maxSets = maxSets;
	pool_info.poolSizeCount = (uint32_t)poolSizes.get_capacity();
	pool_info.pPoolSizes = poolSizes.get_buffer();

	vkCreateDescriptorPool(device, &pool_info, nullptr, &pool);
}

void DescriptorAllocator::clear_descriptors(VkDevice device) {
	vkResetDescriptorPool(device, pool, 0);
}

void DescriptorAllocator::destroy_pool(VkDevice device) {
	vkDestroyDescriptorPool(device, pool, nullptr);
}

VkDescriptorSet DescriptorAllocator::allocate(VkDevice device, VkDescriptorSetLayout layout) {
    VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.pNext = nullptr;
    allocInfo.descriptorPool = pool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &layout;

    VkDescriptorSet ds;
    KN_VULKAN_ERR_CHECK(vkAllocateDescriptorSets(device, &allocInfo, &ds));

    return ds;
}

}

