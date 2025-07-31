
#include "kon/core/directory.hpp"
#include "kon/engine/engine.hpp"
#include "kon/resource/resource.hpp"
#include "kon/resource/resource_cache.hpp"
#include "kon/resource/resource_image.hpp"
#include "kon/resource/resource_pack.hpp"
#include "kon/resource/resource_shader.hpp"
#include <kon/debug/log.hpp>

using namespace kon;

int main() {
	EngineCreateInfo info {
		40000000,  // 40MB
		200000000, // 200MB
		16000000   // 16MB
	};
	Engine engine(info);

	ResourceCache cache(engine.get_allocator_persistent(), &engine);
	engine.mem_dump();
	ResourcePack *pack = cache.add_resource<ResourcePack>(Directory(
				"../core/resources/kon_primitaves/",
				engine.get_allocator_persistent()),
			"primitives");
	ResourceLoadError error {kon::ResourceLoadError_None};
	cache.load_metadata("primitives");
	cache.add_resource_pack("primitives");

	engine.mem_dump();

	cache.load_metadata_group(pack->get_instance_id());
	cache.load_resource_group(pack->get_instance_id());

	engine.mem_dump();

	ResourceShader *shader = cache.get_resource<ResourceShader>("triangle.frag.spv");
	KN_CORE_TRACE("shader {}", shader->get_size());

	ResourceImage *image = cache.get_resource<ResourceImage>("Prototype_Grid_Green_09-256x256.png");
	KN_CORE_TRACE("image {} {}", image->get_metadata().size.x, image->get_metadata().size.y);
}

