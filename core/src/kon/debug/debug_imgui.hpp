#ifndef KN_DEBUG_IMGUI
#define KN_DEBUG_IMGUI

#include "kon/engine/engine.hpp"
namespace kon {

/*
 * this class defines tools for rendering engine types
 * with imgui. this class WILL be shipped with the application
 */
class DebugImgui {
public:
	static void draw_engine_config(Engine *engine);

	/*
	 * forward declared template that our
	 * modules fill out in their own classes
	 */
	// template<class M>
	// static void render_class_debug(M *object);

	template<typename T>
	static void render_reflection();
};

}

#endif
