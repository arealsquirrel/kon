
#include "vulkan_descriptors.hpp"
#include "kon/container/arraylist.hpp"
#include "kon/debug/log.hpp"
#include "modules/graphics/vulkan/vulkan_context.hpp"
#include <vulkan/vulkan_core.h>

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

DescriptorAllocator::DescriptorAllocator(Allocator *allocator, VulkanContext *context) 
	: m_allocator(allocator), m_context(context),
		m_ratios(allocator), m_fullPools(allocator), m_readyPools(allocator) {}
	
DescriptorAllocator::~DescriptorAllocator() = default;

void DescriptorAllocator::init(uint32_t maxSets, const ArrayList<PoolSizeRatio> &poolRatios) {
	m_ratios.reset();
    
	poolRatios.for_each([&](auto &r){
        m_ratios.add(r);
	});
	
    VkDescriptorPool newPool = create_pool(maxSets, poolRatios);

    m_setsPerPool = maxSets * 1.5;

    m_readyPools.add(newPool);
}

void DescriptorAllocator::clear_pools() {
    m_readyPools.for_each([&](auto p){
		vkResetDescriptorPool(m_context->get_device(), p, 0);
    });

    m_fullPools.for_each([&](auto p){
        vkResetDescriptorPool(m_context->get_device(), p, 0);
        m_readyPools.add(p);
    });

    m_fullPools.reset();
}

void DescriptorAllocator::destroy_pools() {
	m_readyPools.for_each([&](auto p){
		vkDestroyDescriptorPool(m_context->get_device(), p, nullptr);
    });

	m_readyPools.reset();

    m_fullPools.for_each([&](auto p){
        vkDestroyDescriptorPool(m_context->get_device(), p, nullptr);
        m_readyPools.add(p);
    });

    m_fullPools.reset();
}

VkDescriptorSet DescriptorAllocator::allocate(VkDescriptorSetLayout layout, void *pNext) {
	VkDescriptorPool poolToUse = get_pool();

	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.pNext = pNext;
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = poolToUse;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = &layout;

	VkDescriptorSet ds;
	VkResult result = vkAllocateDescriptorSets(m_context->get_device(), &allocInfo, &ds);

    //allocation failed. Try again
    if (result == VK_ERROR_OUT_OF_POOL_MEMORY || result == VK_ERROR_FRAGMENTED_POOL) {

        m_fullPools.add(poolToUse);
    
        poolToUse = get_pool();
        allocInfo.descriptorPool = poolToUse;

       KN_VULKAN_ERR_CHECK( vkAllocateDescriptorSets(m_context->get_device(), &allocInfo, &ds));
    }
  
    m_readyPools.add(poolToUse);
    return ds;
}

VkDescriptorPool DescriptorAllocator::get_pool() {
	VkDescriptorPool newPool;
    if (m_readyPools.get_count() != 0) {
        newPool = m_readyPools[m_readyPools.get_count()-1];
        m_readyPools.erase(m_readyPools.get_count()-1);
    } else {
	    newPool = create_pool(m_setsPerPool, m_ratios);

	    m_setsPerPool = m_setsPerPool * 1.5;
	    if (m_setsPerPool > 4092) {
		    m_setsPerPool = 4092;
	    }
    }   

    return newPool;
}

VkDescriptorPool DescriptorAllocator::create_pool(
		uint32_t setCount, const ArrayList<PoolSizeRatio> &poolRatios) {
	
	ArrayList<VkDescriptorPoolSize> poolSizes(m_allocator, poolRatios.get_capacity());
	poolRatios.for_each([&](PoolSizeRatio &ratio){
		poolSizes.add(VkDescriptorPoolSize{
			.type = ratio.type,
			.descriptorCount = uint32_t(ratio.ratio * setCount)
		});
	});

	VkDescriptorPoolCreateInfo pool_info = {};
	pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	pool_info.flags = 0;
	pool_info.maxSets = setCount;
	pool_info.poolSizeCount = (uint32_t)poolSizes.get_capacity();
	pool_info.pPoolSizes = poolSizes.get_buffer();

	VkDescriptorPool newPool;
	vkCreateDescriptorPool(m_context->get_device(), &pool_info, nullptr, &newPool);
    return newPool;
}

DescriptorWriter::DescriptorWriter(Allocator *allocator, VulkanContext *context) 
	: m_allocator(allocator), m_context(context),
	  m_imageInfos(allocator), m_bufferInfos(allocator), m_writes(allocator) {}

DescriptorWriter::~DescriptorWriter() = default;

void DescriptorWriter::write_image(
		int binding, VkImageView image, 
		VkSampler sampler , VkImageLayout layout, VkDescriptorType type) {
	
	VkDescriptorImageInfo& info = m_imageInfos.add(VkDescriptorImageInfo{
		.sampler = sampler,
		.imageView = image,
		.imageLayout = layout
	});

	VkWriteDescriptorSet write = {};
	write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	// = { .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET };

	write.dstBinding = binding;
	write.dstSet = VK_NULL_HANDLE; //left empty for now until we need to write it
	write.descriptorCount = 1;
	write.descriptorType = type;
	write.pImageInfo = &info;

	m_writes.add(write);
}

void DescriptorWriter::write_buffer(
		int binding, VkBuffer buffer, 
		size_t size, size_t offset, VkDescriptorType type) {

	VkDescriptorBufferInfo& info = m_bufferInfos.add(VkDescriptorBufferInfo{
		.buffer = buffer,
		.offset = offset,
		.range = size
		});

	VkWriteDescriptorSet write = {};
	write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	write.dstBinding = binding;
	write.dstSet = VK_NULL_HANDLE; //left empty for now until we need to write it
	write.descriptorCount = 1;
	write.descriptorType = type;
	write.pBufferInfo = &info;

	m_writes.add(write);
}

void DescriptorWriter::clear() {
	m_imageInfos.reset();
	m_bufferInfos.reset();
	m_writes.reset();
}

void DescriptorWriter::update_set(VkDescriptorSet set) {
    m_writes.for_each([&](auto &write){
		write.dstSet = set;
    });

    vkUpdateDescriptorSets(m_context->get_device(),
			(uint32_t)m_writes.get_count(), m_writes.get_buffer(),
			0, nullptr);
}

}

