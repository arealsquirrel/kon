
#include "resource.hpp"

namespace kon {

Resource::Resource(Allocator *allocator,
		Directory path, ShortString name,
		u32 size, UUID groupID)
	: m_allocator(allocator), m_path(path), m_name(name),
	  m_size(size), m_groupID(groupID), m_instanceID() {}

Resource::~Resource() {

}


}

