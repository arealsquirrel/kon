
#include "graphics_module.hpp"
#include "kon/core/object.hpp"

namespace kon {

GraphicsModule::GraphicsModule(Engine *engine, Allocator *allocator)
	: Module(engine, allocator), m_context(engine) {}

GraphicsModule::~GraphicsModule() = default;

void GraphicsModule::init() {
	m_context.init_vulkan();
}

void GraphicsModule::clean() {
	m_context.clean_vulkan();
}


void GraphicsModule::update() {
	m_context.start_frame();
	m_context.draw_clear({0.8f, 0.5f, 0.0f, 1.0f});
	m_context.end_frame();

	m_context.present();
}

}

