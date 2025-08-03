
#include "kon/core/allocator.hpp"
#include "kon/core/events.hpp"
#include "kon/core/object.hpp"
#include <gtest/gtest.h>

#include <kon/core/core.hpp>
#include <kon/engine/engine.hpp>
#include <kon/engine/module.hpp>

using namespace kon;


class FooEvent : public Event {
KN_EVENT

public:
	FooEvent(int _x)
		: x(_x) {}
	~FooEvent() = default;

	int x;
};

class BarEvent : public Event {
KN_EVENT

public:
	BarEvent(int _x)
		: x(_x) {}
	~BarEvent() = default;

	int x;
};

class ModuleFoo : public Module, public EventListener {
KN_OBJECT(ModuleFoo, Module)

public:
	ModuleFoo(Engine *e, Allocator *all)
		: Module(e, all), EventListener(this) {
	
		subscribe_event<FooEvent>([&](FooEvent &event){
			x = 50;
		});
	}

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

class ModuleBar : public Module, public EventListener {
KN_OBJECT(ModuleBar, Module)

public:
	ModuleBar(Engine *e, Allocator *all)
		: Module(e, all), EventListener(this) {

		subscribe_event<BarEvent>([&](BarEvent &event){
			y = 100;
		});
	};

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


TEST(DISABLED_Engine, module) {
	EngineCreateInfo info {
		400000,
		400000,
		400000
	};

	Engine engine(info);

	engine.get_modules().add_module<ModuleFoo>();
	engine.get_modules().add_module<ModuleBar>();

	
	engine.get_modules().init_modules();
	engine.get_modules().update_modules();
	engine.get_modules().clean_modules();
	
	EXPECT_EQ(engine.get_modules().get<ModuleFoo>()->x, 12);
	EXPECT_EQ(engine.get_modules().get<ModuleBar>()->y, 3);
}

TEST(DISABLED_Engine, event) {
	EngineCreateInfo info {
		400000,
		400000,
		400000,
	};

	Engine engine(info);

	
	engine.get_modules().add_module<ModuleFoo>();
	engine.get_modules().add_module<ModuleBar>();

	
	engine.get_event_bus().emit_event<FooEvent>(3);
	EXPECT_EQ(engine.get_modules().get<ModuleFoo>()->x, 50);
	EXPECT_EQ(engine.get_modules().get<ModuleBar>()->y, 0);

	engine.get_event_bus().emit_event<BarEvent>(3);
	EXPECT_EQ(engine.get_modules().get<ModuleFoo>()->x, 50);
	EXPECT_EQ(engine.get_modules().get<ModuleBar>()->y, 100);
}

