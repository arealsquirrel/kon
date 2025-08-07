
#include "resource_shader.hpp"
#include "kon/core/directory.hpp"
#include "kon/resource/resource.hpp"
#include "modules/graphics/graphics_module.hpp"

namespace kon {

ResourceShader::ResourceShader(Allocator *allocator, Engine *engine,
			 Directory path, ShortString name, UUID groupID)
	: Resource(allocator, engine, path, name, groupID) {}
	  // m_shader(m_engine->get_modules().get<GraphicsModule>()->get_graphics_context()){}

ResourceShader::~ResourceShader() {

}

void ResourceShader::load_resource(ResourceLoadError &error) {
	Pair<char*,u32> bfile = read_file_bytes(error, m_path.get_string().c_str());
	m_code = bfile.first;
	m_size = bfile.second;

	Resource::load_resource(error);
}

void ResourceShader::load_metadata(ResourceLoadError &error) {
	// no metadata for this resource
	Resource::load_metadata(error);
}

void ResourceShader::unload_resource() {
	if(m_loadState != ResourceLoadState_FullyLoaded)
		return;

	m_allocator->free_mem(m_code, m_size);

	Resource::unload_resource();
}

}
