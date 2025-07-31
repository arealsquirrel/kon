
#include "kon/core/assert.hpp"
#include "kon/core/directory.hpp"
#include "kon/core/library.hpp"
#include "kon/debug/timer.hpp"
#include "kon/engine/engine.hpp"
#include "kon/resource/resource.hpp"
#include "kon/resource/resource_image.hpp"
#include "kon/resource/resource_model.hpp"
#include "kon/resource/resource_pack.hpp"
#include "kon/resource/resource_shader.hpp"
#include <chrono>
#include <kon/debug/log.hpp>

using namespace kon;

int main() {
	EngineCreateInfo info {
		400000,
	};
	Engine engine(info);
	ResourceLoadError error = ResourceLoadError_None;

	Directory imagePath(String("../core/resources/kon_primitaves/textures/Cubemap_Rocky_01-512x512.png",
				engine.get_allocator_engine()));
	ResourceImage image(engine.get_allocator_engine(), &engine, imagePath, "cubemap_1", 0);
	image.load_metadata(&error);
	image.load_resource(&error);
	KN_CORE_INFO("image {} {}", image.get_metadata().size.x, image.get_metadata().size.x);
	image.unload_resource();

	Directory shaderPath(String("../core/resources/kon_primitaves/shaders/bin/triangle.frag.spv",
				engine.get_allocator_engine()));
	ResourceShader shader(engine.get_allocator_engine(), &engine, shaderPath, "triangle.frag", 0);
	shader.load_metadata(&error);
	shader.load_resource(&error);
	KN_CORE_INFO("shader {}", shader.get_size());
	shader.unload_resource();
	

	engine.mem_dump();
	
	Directory modelPath(String("../core/resources/kon_primitaves/models/cube.obj",
				engine.get_allocator_engine()));
	ResourceModel model(engine.get_allocator_engine(), &engine, modelPath, "cube", 0);
	model.load_metadata(&error);
	model.load_resource(&error);
	model.unload_resource();
	
	engine.mem_dump();
	
	
	
	Directory packPath(String("../core/resources/kon_primitaves/",
				engine.get_allocator_engine()));
	ResourcePack pack(engine.get_allocator_engine(), &engine, packPath, "", 0);

	
	pack.load_metadata(&error);
	
	pack.load_resource(&error);
	pack.unload_resource();

	
	ArrayList<Directory> dirs(engine.get_allocator_engine(), 20);
	platform::iterate_directory(engine.get_allocator_engine(),
			(pack.get_path() + pack.get_textures_path().c_str()).get_string().c_str(),
			dirs);

	dirs.for_each([](Directory &dir, u32){
		KN_CORE_INFO("{}", dir.get_string().c_str());
	});	

	engine.mem_dump();
	
	pack.unload_resource();
}

