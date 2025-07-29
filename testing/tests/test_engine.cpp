
#include "kon/core/allocator.hpp"
#include "kon/core/object.hpp"
#include <gtest/gtest.h>

#include <kon/core/core.hpp>
#include <kon/engine/engine.hpp>
#include <kon/engine/module.hpp>

using namespace kon;

class ModuleFoo : public Module {
KN_OBJECT(ModuleFoo, Module)

public:
	ModuleFoo(Engine *e, Allocator*)
		: Module(e, nullptr) {};
	~ModuleFoo() = default;

	void init() override {
		x += 3;
	}
	void clean() override {
		x += 10;
	}
	void update() override {
		x -= 2;
	}

	int x = 1;
};

class ModuleBar : public Module {
KN_OBJECT(ModuleBar, Module)

public:
	ModuleBar(Engine *e, Allocator*)
		: Module(e, nullptr) {};
	~ModuleBar() = default;

	void init() override {
		y += 1;
	}
	void clean() override {
		y += 4;
	}
	void update() override {
		y -= 2;
	}

	int y = 0;
};

TEST(Engine, module) {
	EngineCreateInfo info;
	info.memoryBudget = 10000;
	Engine engine(info);

	
	engine.get_modules().add_module<ModuleFoo>();
	engine.get_modules().add_module<ModuleBar>();

	
	engine.get_modules().init_modules();
	engine.get_modules().update_modules();
	engine.get_modules().clean_modules();
	
	EXPECT_EQ(engine.get_modules().get<ModuleFoo>()->x, 12);
	EXPECT_EQ(engine.get_modules().get<ModuleBar>()->y, 3);
}
