#ifndef KN_MODULE_HPP
#define KN_MODULE_HPP

#include "kon/container/arraylist.hpp"
#include "kon/core/allocator.hpp"
#include "kon/core/object.hpp"
#include "kon/debug/log.hpp"
#include <kon/core/core.hpp>
#include <utility>

namespace kon {

/*
 * acts as a engine context singleton that only ever has one instance
 * for the engines lifetime. These house API's and their implementations
 */
class Module : public Object {
KN_OBJECT(Module, Object)

public:
	Module(Engine *engine, Allocator *allocator);
	~Module();

	virtual void init() = 0;
	virtual void clean() = 0;
	virtual void update() = 0;
};

// please dont ever copy this :(
class ModuleArray : public Object {
public:
	ModuleArray(Engine *engine, Allocator *allocator);
	~ModuleArray();
	
public:
	void init_modules();
	void clean_modules();
	void update_modules();
	void reload_modules();

public:
	template<class T, typename ...Args>
	T *add_module(Args &&...args) {
		T *mod = m_allocator->allocate<T>(1, m_engine, m_allocator, std::forward<Args>(args)...);
		m_modules.add(dynamic_cast<Module*>(mod));
		return mod;
	}

	template<class T>
	T *get() {
		for(u32 i = 0; i < m_modules.get_count(); i++) {
			if(m_modules[i]->get_type_UUID() == T::get_static_typeinfo()->typeID) {
				return dynamic_cast<T*>(m_modules[i]);
			}
		}

		return nullptr;
	}

private:
	ArrayList<Module*> m_modules;
};

}

#endif
