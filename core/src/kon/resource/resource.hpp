#ifndef KN_RESOURCE_HPP
#define KN_RESOURCE_HPP

#include "kon/container/string.hpp"
#include "kon/core/allocator.hpp"
#include "kon/core/directory.hpp"
#include "kon/core/uuid.hpp"
#include <kon/core/core.hpp>

namespace kon {

enum ResourceLoadError {
	ResourceLoadError_None = 0,

	// returned if the directory is invalid
	ResourceLoadError_BadPath,

	// the format of the file is wrong
	ResourceLoadError_BadFormat,

	// the API used to load the file messed up
	ResourceLoadError_APIError
};

/*
 * this class is not managing the internal state of the resource
 * that job is for the resource cache. The idea here is to 
 * focus on implementing the resource rather than having to fuck around with state
 */
class Resource {
public:
	// the size paramater is the size of the resource in bytes
	Resource(Allocator *allocator,
			Directory path, ShortString name,
			u32 size, UUID groupID=0);
	virtual ~Resource();

	Directory get_path() const { return m_path; }
	ShortString get_name() const { return m_name; }
	UUID get_group_uuid() const { return m_groupID; }
	UUID get_instance_uuid() const { return m_instanceID; }
	u32 get_size() const { return m_size; }

public:
	/*
	 * loads the memory of the resource off the computer
	 * EX: loads the image into memory
	 */
	virtual void load_resource(ResourceLoadError *error) = 0;
	
	/*
	 * loads data about the memory into memory
	 * EX: loads the image size and channels into memory
	 */
	virtual void load_metadata(ResourceLoadError *error) = 0;

	/*
	 * unloads the resource from memory,
	 * keeps the metadata
	 */
	virtual void unload_resource(ResourceLoadError *error) = 0;
	
public:


private:
	Allocator *m_allocator;
	Directory m_path;
	ShortString m_name;
	u32 m_size;
	UUID m_groupID;
	UUID m_instanceID;
};

}

#endif
