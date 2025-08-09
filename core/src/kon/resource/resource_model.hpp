#ifndef KN_RESOURCE_MODEL
#define KN_RESOURCE_MODEL

#include "kon/core/directory.hpp"
#include "kon/core/object.hpp"
#include "kon/engine/engine.hpp"
#include "kon/resource/resource.hpp"
#include "modules/graphics/graphics_module.hpp"
#include "modules/graphics/vulkan/vulkan_buffer.hpp"
#include <kon/core/core.hpp>
#include <kon/math/vector_operations.hpp>

namespace kon {


class ResourceModel : public Resource {
KN_OBJECT(ResourceModel, Resource)

public:
	ResourceModel(Allocator *allocator, Engine *engine,
			 Directory path, ShortString name, UUID groupID=UUID(0));
	~ResourceModel();

	void load_resource(ResourceLoadError &error) override;
	void load_metadata(ResourceLoadError &error) override;
	void unload_resource() override;

public:
	const ArrayList<Vertex> &get_verticies() const { return m_verticies; }
	const ArrayList<u32> &get_indices() const { return m_indices; }
	inline VulkanMeshBuffer *get_mesh() { return &m_mesh; }

private:
	ArrayList<Vertex> m_verticies;
	ArrayList<u32> m_indices;

	VulkanMeshBuffer m_mesh;
};

}

namespace std {
    template<> struct hash<kon::Vertex> {
        size_t operator()(kon::Vertex const& vertex) const {
            return ((kon::hash_vector(vertex.position) ^ (kon::hash_vector(vertex.color)) << 1));
        }
    };
}

#endif
