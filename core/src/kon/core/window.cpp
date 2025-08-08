
#include "window.hpp"
#include "kon/core/events.hpp"
#include "kon/core/object.hpp"
#include "kon/debug/log.hpp"
#include <kon/engine/engine.hpp>
#include <GLFW/glfw3.h>

namespace kon {

Engine *Window::s_engine = nullptr;

void error_callback(int, const char* description) {
	KN_CORE_ERROR("GLFW ERROR {}", description);
}


Window::Window(Engine *engine, WindowCreateInfo info) 
	: Object(engine), m_width(info.width), m_height(info.height) {

	if(s_engine == nullptr) {
		KN_CORE_INFO("making this window the current context");
		init_glfw();
		s_engine = engine;
	}

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	m_window = glfwCreateWindow(info.width, info.height, info.name.c_str(), NULL, NULL);
	if (!m_window) {
		KN_CORE_ERROR("glfwCreateWindow did uh not do that");
		return;
	}

	glfwSetWindowCloseCallback(m_window, [](GLFWwindow*){
		Window::s_engine->get_event_bus().emit_event<EventEngineExit>();
	});

	glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow *window, int width, int height){
		KN_TRACE("resize {} {}", width, height);
		Window::s_engine->get_event_bus().emit_event<EventWindowResize>(width, height);
	});
}

Window::~Window() {
	glfwDestroyWindow(m_window);
}

void Window::init_glfw() {
	// glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
	if(glfwInit() == GLFW_FALSE) {
		KN_CORE_ERROR("glfwInit failed :(");
	}

	glfwSetErrorCallback(error_callback);
}

}

