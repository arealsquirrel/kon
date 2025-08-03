
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
	ResourceLoadError error = ResourceLoadError_None;
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
	KN_CORE_TRACE("Loading resource pack {}", pack->get_name().c_str());

	// ------------ REGISTER TEXTURES ------------
	KN_CORE_TRACE("adding textures");
	ArrayList<Directory> dirs(m_allocator, 30); // prefire a cool 30 elements
	platform::iterate_directory(m_allocator, (pack->get_path() + pack->get_textures_path()).c_str(), dirs);
	dirs.for_each([&](Directory &dir, u32){
		if(dir.get_file_name().equals(".") ||
		   dir.get_file_name().equals("..") ||
		   dir.get_stat().directory) return;
		
		auto fullPath = (pack->get_path() + pack->get_textures_path() + dir);
		KN_CORE_TRACE("\t{}", dir.get_string().c_str());

		if(fullPath.get_valid() == false) {
			KN_CORE_WARN("path {} is invalid", fullPath.c_str());
		}

		add_resource<ResourceImage>(fullPath, "", pack->get_instance_id());
	});
	dirs.reset();
	dirs.resize(30);
	
	// ------------ REGISTER MODELS ------------
	/*
	KN_CORE_TRACE("adding models {}", (pack->get_path() + pack->get_models_path()).c_str());
	platform::iterate_directory(m_allocator, (pack->get_path() + pack->get_models_path()).c_str(), dirs);
	
	dirs.for_each([&](Directory &dir, u32){

		if(dir.get_file_name().equals(".") ||
		   dir.get_file_name().equals("..") ||
		   dir.get_stat().directory) {

			KN_CORE_TRACE("{}", dir.c_str());
		}
	
		auto fullPath = (pack->get_path() + pack->get_models_path() + dir);
		KN_CORE_TRACE("\t{}", dir.get_string().c_str());

		if(fullPath.get_valid() == false) {
			KN_CORE_WARN("path {} is invalid", fullPath.c_str());
		}

		add_resource<ResourceModel>(fullPath, dir.c_str(), pack->get_instance_id());
	});

	dirs.reset();
	dirs.resize(30);
	

	// ------------ REGISTER SHADERS ------------
	KN_CORE_TRACE("adding shaders");
	platform::iterate_directory(m_allocator, (pack->get_path() + pack->get_shaders_path()).c_str(), dirs);
	dirs.for_each([&](Directory &dir, u32){
		if(dir.get_file_name().equals(".") ||
		   dir.get_file_name().equals("..") ||
		   dir.get_stat().directory) return;
		
		auto fullPath = (pack->get_path() + pack->get_shaders_path() + dir);
		KN_CORE_TRACE("\t{}", dir.get_string().c_str());

		if(fullPath.get_valid() == false) {
			KN_CORE_WARN("path {} is invalid", fullPath.c_str());
		}

		add_resource<ResourceShader>(fullPath, dir.c_str(), pack->get_instance_id());
	});
	*/

	KN_CORE_TRACE("done loading resource pack");
}

void ResourceCache::unload_resource(ShortString name) {
	Resource *r = get_resource(name);
	r->unload_resource();
}

void ResourceCache::unload_resource_group(UUID group) {
	m_stringToResource.for_each([&](auto &p){
		if(p.second->get_group_uuid() == group || group == groupIDAll) {
			if(p.second->get_load_state() == ResourceLoadState_Unloaded)
				return;

			KN_CORE_TRACE("unloading resource {}", p.second->get_name().c_str());
			p.second->unload_resource();
		}
	});
}

}

