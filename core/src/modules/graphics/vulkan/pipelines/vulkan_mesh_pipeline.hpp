#ifndef KN_VULKAN_MESH_PIPELINE_HPP
#define KN_VULKAN_MESH_PIPELINE_HPP

#include "kon/math/matrix4x4.hpp"
#include "modules/graphics/vulkan/vulkan_buffer.hpp"
#include "modules/graphics/vulkan/vulkan_pipeline.hpp"
#include <vulkan/vulkan_core.h>

namespace kon {

class VulkanContext;
class VulkanBuffer;
class Engine;

class VulkanMeshPipeline : public VulkanPipeline {
public:
	// push constants for our mesh object draws
	struct PushConstants {
		Matrix4x4 worldMatrix;
		VkDeviceAddress vertexBuffer;
		int renderMode;
	};

	struct SceneData {
		Matrix4x4 view;
		Matrix4x4 proj;
		Matrix4x4 ambientColor;
		Matrix4x4 sunlightDirection;
		Matrix4x4 sunlightColor;
	};

public:
	VulkanMeshPipeline(Engine *engine, VulkanContext *context);
	~VulkanMeshPipeline();

	void create(Allocator *allocator);
	void destroy();

	inline VkDescriptorSetLayout get_layout() { return m_descriptorImage; };
	inline VkDescriptorSetLayout get_image_layout() { return m_descriptorImage; }
	inline VkPipelineLayout get_pipeline_layout() { return m_layout; }

public:
	void bind_pipeline(VkCommandBuffer cmd) override;
	void bind_descriptor_sets(VkCommandBuffer cmd, VulkanBuffer *buffer) override;
	void bind_push_constants(VkCommandBuffer cmd, char *data) override;
	void draw(VkCommandBuffer cmd) override;

public:
	SceneData m_sceneData;

private:
	VkPipelineLayout m_layout;
	VkPipeline m_pipeline;
	VkDescriptorSet m_descriptors;
	VkDescriptorSetLayout m_descriptorLayout;
	VkDescriptorSetLayout m_descriptorImage;

	VulkanContext *m_context;
	Engine *m_engine;
};

}

#endif
