#ifndef KN_RESOURCE_CACHE_HPP
#define KN_RESOURCE_CACHE_HPP

#include "kon/container/hashmap.hpp"
#include <kon/math/vector2.hpp>
#include "kon/container/string.hpp"
#include "kon/core/allocator.hpp"
#include "kon/core/directory.hpp"
#include "kon/core/object.hpp"
#include "kon/core/uuid.hpp"
#include "kon/resource/resource.hpp"
#include <kon/core/core.hpp>

namespace kon {

/*
 * stores resources and holds logic for loading them and such
 */
class ResourceCache : public Object {
KN_OBJECT(ResourceCache, Object)

public:
	static UUID groupIDAll;

public:
	ResourceCache(Allocator *allocator, Engine *engine);
	~ResourceCache();

public:
	template<class R>
	R *add_resource(Directory path, ShortString name, UUID gid=groupIDAll) {
		R *r = m_allocator->allocate<R>(1, m_allocator, m_engine, path, name, gid);
		m_stringToResource.add({name, dynamic_cast<Resource*>(r)});
		return r;
	}

	// the group id for a resource pack is the packs resource instance id
	void add_resource_pack(ShortString name);

	void load_metadata(ShortString name);
	void load_metadata_group(UUID group);

	void load_resource(ShortString name);
	void load_resource_group(UUID group);

	void unload_resource(ShortString name);
	void unload_resource_group(UUID group);

	Resource *get_resource(ShortString name);

	template<class R>
	R *get_resource(ShortString name) {
		return get_resource(name)->cast<R>();
	}

private:
	void insert_resource(Resource *resource, UUID groupID);

private:
	HashMap<ShortString, Resource*> m_stringToResource;	
};

}

#endif
