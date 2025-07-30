
#include "resource_image.hpp"
#include "kon/core/directory.hpp"
#include "kon/resource/resource.hpp"

namespace kon {

ResourceImage::ResourceImage(Allocator *allocator, Engine *engine,
			 Directory path, ShortString name, UUID groupID)
	: Resource(allocator, engine, path, name, groupID) {}

ResourceImage::~ResourceImage() {

}

void ResourceImage::load_resource(ResourceLoadError *error) {
	Resource::load_resource(error);
}

void ResourceImage::load_metadata(ResourceLoadError *error) {
	Resource::load_metadata(error);
}

void ResourceImage::unload_resource() {
	Resource::unload_resource();
}

}

