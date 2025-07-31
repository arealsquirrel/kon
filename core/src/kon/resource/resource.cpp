
#include "resource.hpp"

#include <fstream>
#include <nlohmann/json.hpp>

namespace kon {

Resource::Resource(Allocator *allocator, Engine *engine,
		Directory path, ShortString name, UUID groupID)
	: Object(engine, allocator), 
	  m_path(path), m_name(name), m_groupID(groupID) {}

Resource::~Resource() {

}

void Resource::load_resource(ResourceLoadError *error) {
	KN_ASSERT(m_loadState != ResourceLoadState_FullyLoaded, 
			"resource already loaded. If you are remaking the resource, please call unload first");

	if(*error != ResourceLoadError_None) {
		KN_CORE_WARN("Resource::load returning with error {}", 
				load_error_to_string(*error));

		m_loadState = ResourceLoadState_Unloaded;
	}

	m_loadState = ResourceLoadState_FullyLoaded;
}

void Resource::load_metadata(ResourceLoadError *error) {
	if(*error != ResourceLoadError_None) {
		KN_CORE_WARN("Resource::load_metadata returning with error {}", 
				load_error_to_string(*error));

		m_loadState = ResourceLoadState_Unloaded;
	}

	m_loadState = ResourceLoadState_Partialy;
}

void Resource::unload_resource() {
	KN_ASSERT(m_loadState != ResourceLoadState_Unloaded, "resource already unloaded");
}

Pair<char*, u32> Resource::read_file_strings(ResourceLoadError *error, const char *path) {
	std::ifstream file(path);

	if(file.fail()) {
		*error = ResourceLoadError_BadPath;
		KN_CORE_WARN("read_file_bytes ifstream failed on file {}", path);
		return {nullptr, 0};
	}

	file.seekg(0, std::ios::end);
	size_t size = file.tellg();
	file.seekg(0);

	char *buffer = m_allocator->allocate_mem(size); 
	file.read(buffer, size);

	return Pair<char*, u32>(buffer, size);
}

Pair<char*, u32> Resource::read_file_bytes(ResourceLoadError *error, const char *path) {
	std::ifstream file(path, std::ios::ate | std::ios::binary);
	
	if(file.fail()) {
		*error = ResourceLoadError_BadPath;
		KN_CORE_WARN("read_file_bytes ifstream failed on file {}", path);
		return {nullptr, 0};
	}
	
	u32 fileSize = (size_t) file.tellg();
	file.seekg(0);
	
	char *buffer = m_allocator->allocate_mem(fileSize);
	file.read(buffer, fileSize);
	return Pair<char*, u32>(buffer, fileSize);
}


nlohmann::json Resource::read_file_json(ResourceLoadError *error, const char *path) {
	std::ifstream file(path);
	nlohmann::json j;

	try {
		j = nlohmann::json::parse(file); 
	} catch (nlohmann::json::exception &exception) {
		KN_CORE_ERROR("nlohmann::json excpetion thrown while reading file {}\n \
				{}",path, exception.what());
		*error = ResourceLoadError_APIError;
	}
	
	return j;
}

}

