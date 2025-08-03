#ifndef KN_OBJECT_HPP
#define KN_OBJECT_HPP

#include "kon/container/string.hpp"
#include "kon/core/uuid.hpp"
#include <kon/core/core.hpp>

namespace kon {

class Engine;

struct TypeInfo {
	const TypeInfo *parent;
	ShortString name;
	UUID typeID;
};

class Object {
public:
	explicit Object(Engine *engine, Allocator *allocator=nullptr);
	virtual ~Object();

public:
	virtual const TypeInfo *get_typeinfo() const { return get_static_typeinfo(); }
	virtual UUID get_type_UUID() const { return get_static_typeinfo()->typeID; }
	static const TypeInfo *get_static_typeinfo() { static TypeInfo info {nullptr, "Object", UUID()}; return &info; }

public:
	UUID get_instance_id() const { return m_instanceID; }
	Allocator *get_allocator() const { return m_allocator; }
	Engine *get_engine() const { return m_engine; }

	/*
	 * T must inherit from object
	 */
	template<typename T>
	bool instance_of() const {
		const TypeInfo *info = get_typeinfo();
		while(info != nullptr) {
			if(info->typeID == T::get_static_typeinfo()->typeID) {
				return true;
			}

			info = info->parent;
		}

		return false;
	}

	template<typename T>
	inline T *cast() {
		KN_ASSERT(instance_of<T>(), "this object {} is not an instance of T", this->get_typeinfo()->name.c_str());
		return dynamic_cast<T*>(this);
	}

	template<typename T>
	inline const T *cast() const {
		KN_ASSERT(instance_of<T>(), "this object {} is not an instance of T", this->get_typeinfo()->name.c_str());
		return dynamic_cast<T*>(this);
	}

protected:
	UUID m_instanceID;
	Engine *m_engine;
	Allocator *m_allocator;
};

#define KN_OBJECT(className, parent) public: \
	virtual const TypeInfo *get_typeinfo() const override { return get_static_typeinfo(); } \
	virtual UUID get_type_UUID() const override { return get_static_typeinfo()->typeID; } \
	static const TypeInfo *get_static_typeinfo() { static TypeInfo info {parent::get_static_typeinfo(), #className, UUID()}; return &info; } \


}

#endif
