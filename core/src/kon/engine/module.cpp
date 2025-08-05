
#include "module.hpp"

namespace kon {

Module::Module(Engine *engine, Allocator *allocator)
	: Object(engine, allocator) {}
	
Module::~Module() = default;

ModuleArray::ModuleArray(Engine *engine, Allocator *allocator) 
	: Object(engine, allocator), m_modules(allocator, 10) {} // that should be enough <3
	
ModuleArray::~ModuleArray() {
	m_modules.for_each([&](Module *m){
		m_allocator->free(m);
	});
}

void ModuleArray::init_modules() {
	m_modules.for_each([](Module *m){
		m->init();
	}); 
}

void ModuleArray::clean_modules() {
	m_modules.for_each([](Module *m){
		m->clean();
	}); 
}

void ModuleArray::update_modules() {
	m_modules.for_each([](Module *m){
		m->update();
	}); 
}

void ModuleArray::reload_modules() {
	clean_modules();
	init_modules();
}

}

