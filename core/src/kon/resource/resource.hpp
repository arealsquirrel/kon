#ifndef KN_RESOURCE_HPP
#define KN_RESOURCE_HPP

#include "kon/container/string.hpp"
#include "kon/core/allocator.hpp"
#include "kon/core/directory.hpp"
#include "kon/core/object.hpp"
#include "kon/core/uuid.hpp"
#include <kon/core/core.hpp>

#include <nlohmann/json_fwd.hpp>

namespace kon {

class Engine;

enum ResourceLoadError {
	ResourceLoadError_None = 0,

	// returned if the directory is invalid
	ResourceLoadError_BadPath,

	// the format of the file is wrong
	ResourceLoadError_BadFormat,

	// the API used to load the file messed up
	ResourceLoadError_APIError
};

enum ResourceLoadState {
	ResourceLoadState_Unloaded,

	/*
	 * this means that the metadata is loaded but not the
	 * full resource
	 */
	ResourceLoadState_Partialy,
	
	/* the full resource */
	ResourceLoadState_FullyLoaded
};

constexpr const char *load_error_to_string(ResourceLoadError error) {
	switch (error) {
		case ResourceLoadError_None: return "none";
		case ResourceLoadError_BadPath: return "bad path";
		case ResourceLoadError_BadFormat: return "bad format";
		case ResourceLoadError_APIError: return "api error";
	}

	return "";
}

constexpr const char *load_state_to_string(ResourceLoadState state) {
	switch(state) {
		case ResourceLoadState_Unloaded: return "unloaded";
		case ResourceLoadState_FullyLoaded: return "fully loaded";
		case ResourceLoadState_Partialy: return "partialy loaded";
	}

	return "";
}

/*
 * this class is not managing the internal state of the resource
 * that job is for the resource cache. The idea here is to 
 * focus on implementing the resource rather than having to fuck around with state
 */
class Resource : public Object {
KN_OBJECT(Resource, Object)

public:
	// the size paramater is the size of the resource in bytes
	Resource(Allocator *allocator, Engine *engine,
			Directory path, ShortString name, UUID groupID=0);
	virtual ~Resource();

	Directory get_path() const { return m_path; }
	ShortString get_name() const { return m_name; }
	UUID get_group_uuid() const { return m_groupID; }
	ResourceLoadState get_load_state() const { return m_loadState; }

public:
	/*
	 * loads the memory of the resource off the computer
	 * EX: loads the image into memory
	 */
	virtual void load_resource(ResourceLoadError *error);
	
	/*
	 * loads data about the memory into memory
	 * EX: loads the image size and channels into memory
	 */
	virtual void load_metadata(ResourceLoadError *error);

	/*
	 * unloads the resource from memory,
	 * keeps the metadata
	 */
	virtual void unload_resource();
	
protected:
	/*
	 * reads a file as characters and returns a char* 
	 * YOU MUST FREE THIS MEMORY YOURSELF
	 */
	Pair<char*, u32> read_file_strings(ResourceLoadError *error, const char *path);
	
	/*
	 * reads a file as bytes and returns a char* 
	 * YOU MUST FREE THIS MEMORY YOURSELF
	 */
	Pair<char*, u32> read_file_bytes(ResourceLoadError *error, const char *path);

	/*
	 * returns the json of the file 
	 */
	nlohmann::json read_file_json(ResourceLoadError *error, const char *path);

protected:
	Directory m_path;
	ShortString m_name;
	UUID m_groupID;

	ResourceLoadState m_loadState;
};

}

#endif
