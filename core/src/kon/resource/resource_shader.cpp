
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
	Resource::load_resource(error);
}

void ResourceShader::load_metadata(ResourceLoadError *error) {
	Resource::load_metadata(error);
}

void ResourceShader::unload_resource() {
	Resource::unload_resource();
}

}
