#ifndef KN_VULKAN_MESH_PIPELINE_HPP
#define KN_VULKAN_MESH_PIPELINE_HPP

#include "kon/math/matrix4x4.hpp"
#include "modules/graphics/vulkan/vulkan_pipeline.hpp"

namespace kon {

class VulkanContext;
class Engine;

class VulkanMeshPipeline : public VulkanPipeline {
public:
	// push constants for our mesh object draws
	struct PushConstants {
		Matrix4x4 worldMatrix;
		VkDeviceAddress vertexBuffer;
		int renderMode;
	};

public:
	VulkanMeshPipeline(Engine *engine, VulkanContext *context);
	~VulkanMeshPipeline();

	void create(Allocator *allocator);
	void destroy();

public:
	void bind_pipeline(VkCommandBuffer cmd) override;
	void bind_descriptor_sets(VkCommandBuffer cmd) override;
	void bind_push_constants(VkCommandBuffer cmd, char *data) override;
	void draw(VkCommandBuffer cmd) override;

private:
	VkPipelineLayout m_layout;
	VkPipeline m_pipeline;
	VkDescriptorSet m_descriptors;
	VkDescriptorSetLayout m_descriptorLayout;

	VulkanContext *m_context;
	Engine *m_engine;
};

}

#endif
