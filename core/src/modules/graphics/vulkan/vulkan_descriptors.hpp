#ifndef KN_VULKAN_DESCRIPTORS
#define KN_VULKAN_DESCRIPTORS

#include "kon/container/arraylist.hpp"
#include "kon/core/allocator.hpp"
#include <vulkan/vulkan_core.h>

namespace kon {

class VulkanContext;

struct DescriptorLayoutBuilder {
    ArrayList<VkDescriptorSetLayoutBinding> bindings;

public:
    void add_binding(uint32_t binding, VkDescriptorType type);
    void clear() { bindings.reset(); }

    VkDescriptorSetLayout build(VkDevice device, VkShaderStageFlags shaderStages, void* pNext = nullptr, VkDescriptorSetLayoutCreateFlags flags = 0);
};

class DescriptorAllocator {
public:
    struct PoolSizeRatio{
		VkDescriptorType type;
		float ratio;
    };

public:
	DescriptorAllocator(Allocator *allocator, VulkanContext *context);
	~DescriptorAllocator();

    void init(uint32_t maxSets, const ArrayList<PoolSizeRatio> &poolRatios);
	void clear_pools();
	void destroy_pools();

    VkDescriptorSet allocate(VkDescriptorSetLayout layout, void *pNext);

private:
	VkDescriptorPool get_pool();
	VkDescriptorPool create_pool(uint32_t setCount, const ArrayList<PoolSizeRatio> &poolRatios);

private:
	Allocator *m_allocator;
	VulkanContext *m_context;


    VkDescriptorPool m_pool;
	ArrayList<PoolSizeRatio> m_ratios;
	ArrayList<VkDescriptorPool> m_fullPools;
	ArrayList<VkDescriptorPool> m_readyPools;
	uint32_t m_setsPerPool;
};

class DescriptorWriter {
public:
	DescriptorWriter(Allocator *allocator, VulkanContext *context);
	~DescriptorWriter();

public:
    void write_image(int binding,VkImageView image,VkSampler sampler , VkImageLayout layout, VkDescriptorType type);
    void write_buffer(int binding,VkBuffer buffer,size_t size, size_t offset,VkDescriptorType type); 

    void clear();
    void update_set(VkDescriptorSet set);

public:
	Allocator *m_allocator;
	VulkanContext *m_context;

	ArrayList<VkDescriptorImageInfo> m_imageInfos;
	ArrayList<VkDescriptorBufferInfo> m_bufferInfos;
	ArrayList<VkWriteDescriptorSet> m_writes;
};

}

#endif
