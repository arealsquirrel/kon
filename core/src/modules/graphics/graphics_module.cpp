
#include "graphics_module.hpp"
#include "kon/core/events.hpp"
#include "kon/core/object.hpp"
#include "kon/debug/debug_imgui.hpp"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

namespace kon {

GraphicsModule::GraphicsModule(Engine *engine, Allocator *allocator)
	: Module(engine, allocator), EventListener(this), m_context(engine) {

	subscribe_event<EventWindowResize>([&](EventWindowResize &event){
		m_context.recreate_swapchain();
	});
}

GraphicsModule::~GraphicsModule() = default;

void GraphicsModule::init() {
	m_context.init_vulkan();
}

void GraphicsModule::clean() {
	m_context.clean_vulkan();
}


void GraphicsModule::update() {
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	//some imgui UI to test
	ImGui::ShowDemoWindow();
	DebugImgui::draw_engine_config(m_engine);

	//make imgui calculate internal draw structures
	ImGui::Render();

	m_context.start_frame();
	m_context.draw_clear({0.8f, 0.5f, 0.0f, 1.0f});
	m_context.end_frame();

	m_context.present();
}

void GraphicsModule::render_debug() {
	ImGui::ColorEdit4("Color 1", &m_context.m_cpsPushConstants.color1.vec[0]);
	ImGui::ColorEdit4("Color 2", &m_context.m_cpsPushConstants.color2.vec[0]);	
}

}

