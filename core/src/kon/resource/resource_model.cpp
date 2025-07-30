
#include "resource_model.hpp"
#include "kon/core/directory.hpp"
#include "kon/resource/resource.hpp"

namespace kon {

ResourceModel::ResourceModel(Allocator *allocator, Engine *engine,
			 Directory path, ShortString name, UUID groupID)
	: Resource(allocator, engine, path, name, groupID) {}

ResourceModel::~ResourceModel() {

}

void ResourceModel::load_resource(ResourceLoadError *error) {
	Resource::load_resource(error);
}

void ResourceModel::load_metadata(ResourceLoadError *error) {
	Resource::load_metadata(error);
}

void ResourceModel::unload_resource() {
	Resource::unload_resource();
}

}

