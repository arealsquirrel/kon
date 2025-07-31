
#include "resource_shader.hpp"
#include "kon/core/directory.hpp"
#include "kon/resource/resource.hpp"

namespace kon {

ResourceShader::ResourceShader(Allocator *allocator, Engine *engine,
			 Directory path, ShortString name, UUID groupID)
	: Resource(allocator, engine, path, name, groupID) {}

ResourceShader::~ResourceShader() {

}

void ResourceShader::load_resource(ResourceLoadError *error) {
	Pair<char*,u32> bfile = read_file_bytes(error, m_path.get_string().c_str());

	m_shader = bfile.first;
	m_size = bfile.second;

	Resource::load_resource(error);
}

void ResourceShader::load_metadata(ResourceLoadError *error) {
	// no metadata for this resource
	Resource::load_metadata(error);
}

void ResourceShader::unload_resource() {
	m_allocator->free_mem(m_shader, m_size);
	Resource::unload_resource();
}

}
