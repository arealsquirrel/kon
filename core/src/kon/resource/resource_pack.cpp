
#include "resource_pack.hpp"
#include "kon/core/directory.hpp"
#include "kon/debug/log.hpp"
#include "kon/resource/resource.hpp"

namespace kon {

ResourcePack::ResourcePack(Allocator *allocator, Engine *engine,
			 Directory path, ShortString name, UUID groupID)
	: Resource(allocator, engine, path, name, groupID) {}

ResourcePack::~ResourcePack() = default;

void ResourcePack::load_resource(ResourceLoadError &error) {
	Resource::load_resource(error);
}

void ResourcePack::load_metadata(ResourceLoadError &error) {
	m_json = read_file_json(error, (m_path + "pack.json").get_string().c_str());
	m_shaders = m_json["shaders_path"].template get<std::string>().c_str();
	m_textures = m_json["textures_path"].template get<std::string>().c_str();
	m_models = m_json["models_path"].template get<std::string>().c_str();

	Resource::load_metadata(error);
}

void ResourcePack::unload_resource() {
	Resource::unload_resource();
}

}

