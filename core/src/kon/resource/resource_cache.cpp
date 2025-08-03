
#include "resource_cache.hpp"
#include "kon/debug/log.hpp"
#include "kon/resource/resource.hpp"
#include "kon/resource/resource_image.hpp"
#include "kon/resource/resource_model.hpp"
#include "kon/resource/resource_pack.hpp"
#include "kon/resource/resource_shader.hpp"

namespace kon {

UUID ResourceCache::groupIDAll = UUID(0);

ResourceCache::ResourceCache(Allocator *allocator, Engine *engine) 
	: Object(engine, allocator), m_stringToResource(allocator, 100) {}

ResourceCache::~ResourceCache() {
	m_stringToResource.for_each([&](auto &pair){
		m_allocator->free(pair.second);
	});
}

Resource *ResourceCache::get_resource(ShortString name) {
	return m_stringToResource.find_entry(name).second;
}

void ResourceCache::load_metadata(ShortString name) {
	Resource *r = get_resource(name);
	ResourceLoadError error{ResourceLoadError_None};
	r->load_metadata(&error);

	if(error != ResourceLoadError_None) {
		KN_CORE_ERROR("Resource load metadata failed {}", load_error_to_string(error));
	}
}

void ResourceCache::load_metadata_group(UUID group) {
	m_stringToResource.for_each([&](auto &p){
		if(p.second->get_group_uuid() == group || group == groupIDAll) {
			KN_CORE_TRACE("loading metadata {}", p.second->get_name().c_str());
			ResourceLoadError error{ResourceLoadError_None};
			p.second->load_metadata(&error);

			if(error != ResourceLoadError_None) {
				KN_CORE_ERROR("Resource load metadata failed {}", load_error_to_string(error));
			}
		}
	});
}

void ResourceCache::load_resource(ShortString name) {
	Resource *r = get_resource(name);
	ResourceLoadError error{ResourceLoadError_None};
	r->load_resource(&error);

	if(error != ResourceLoadError_None) {
		KN_CORE_ERROR("Resource load metadata failed {}", load_error_to_string(error));
	}
}

void ResourceCache::load_resource_group(UUID group) {
	m_stringToResource.for_each([&](auto &p){
		if(p.second->get_group_uuid() == group || group == groupIDAll) {
			KN_CORE_TRACE("loading resource {}", p.second->get_name().c_str());
			ResourceLoadError error{ResourceLoadError_None};
			p.second->load_resource(&error);

			if(error != ResourceLoadError_None) {
				KN_CORE_ERROR("Resource load failed {}", load_error_to_string(error));
			}
		}
	});
}

void ResourceCache::add_resource_pack(ShortString name) {
	ResourcePack *pack = m_stringToResource.find_entry(name).second->cast<ResourcePack>();
	
	// ------------ REGISTER TEXTURES ------------
	ArrayList<Directory> dirs(m_allocator, 30); // prefire a cool 30 elements
	platform::iterate_directory(m_allocator,
			(pack->get_path() + pack->get_textures_path()).c_str(), dirs);

	dirs.for_each([&](Directory &dir, u32){
		if(dir.get_file_name().equals(".") || dir.get_file_name().equals("..") || dir.get_stat().directory) return;

		KN_CORE_INFO("pack {}", dir.get_string().c_str());

		if(dir.get_valid() == false) {
			KN_CORE_WARN("ResourcePack {} path {} is invalid", name.c_str(), dir.c_str());
		}

		add_resource<ResourceImage>(dir, dir.get_file_name(), pack->get_instance_id());
	});

	dirs.reset();
	dirs.resize(30);
	
	// ------------ REGISTER MODELS ------------
	platform::iterate_directory(m_allocator,
			(pack->get_path() + pack->get_models_path()).c_str(), dirs);

	dirs.for_each([&](Directory &dir, u32) {
		if(dir.get_file_name().equals(".") || dir.get_file_name().equals("..") || dir.get_stat().directory) return;

		KN_CORE_INFO("pack {}", dir.get_string().c_str());

		if(dir.get_valid() == false) {
			KN_CORE_WARN("ResourcePack {} path {} is invalid", name.c_str(), dir.c_str());
		}

		add_resource<ResourceModel>(dir, dir.get_file_name(), pack->get_instance_id());
	});
	dirs.reset();
	dirs.resize(30);

	// ------------ REGISTER SHADERS ------------
	platform::iterate_directory(m_allocator,
			(pack->get_path() + pack->get_shaders_path()).c_str(), dirs);

	dirs.for_each([&](Directory &dir, u32){
		if(dir.get_file_name().equals(".") || dir.get_file_name().equals("..") || dir.get_stat().directory) return;
		
		KN_CORE_INFO("pack {}", dir.get_string().c_str());

		if(dir.get_valid() == false) {
			KN_CORE_WARN("ResourcePack {} path {} is invalid", name.c_str(), dir.c_str());
		}

		add_resource<ResourceShader>(dir, dir.get_file_name(), pack->get_instance_id());
	});
}

void ResourceCache::unload_resource(ShortString name) {
	Resource *r = get_resource(name);
	r->unload_resource();
}

void ResourceCache::unload_resource_group(UUID group) {
	m_stringToResource.for_each([&](auto &p){
		if(p.second->get_group_uuid() == group || group == groupIDAll) {
			KN_CORE_TRACE("unloading resource {}", p.second->get_name().c_str());
			p.second->unload_resource();
		}
	});
}

}

