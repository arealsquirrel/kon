#ifndef KN_WINDOW_HPP
#define KN_WINDOW_HPP

#include "kon/core/object.hpp"
#include <kon/core/core.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace kon {

class Engine;

struct WindowCreateInfo {
	ShortString name;
	u16 width;
	u16 height;
};

class Window : public Object {
KN_OBJECT(Window, Object)

public:
	Window(Engine *engine, WindowCreateInfo info);
	~Window();

	GLFWwindow *get_handle() const { return m_window; }
	u32 get_width() const { return m_width; }
	u32 get_height() const { return m_height; }

	void poll_events() const { glfwPollEvents(); }

public:
	static void init_glfw();

private:
	GLFWwindow *m_window;
	u32 m_width;
	u32 m_height;

	/*
	 * we have to bind an engine because of the way
	 * GLFW events work
	 */
	static Engine *s_engine;
};

}

#endif
