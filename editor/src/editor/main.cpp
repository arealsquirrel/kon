
#include "kon/core/events.hpp"
#include "kon/engine/engine.hpp"
#include "kon/resource/resource.hpp"
#include "kon/resource/resource_pack.hpp"
#include <kon/debug/log.hpp>
#include <kon/debug/instrumentation.hpp>

using namespace kon;

int main() {
	KN_INSTRUMENT_NEW_FILE("logs/startup.json");

	EngineCreateInfo info {
		40000000,  // 40MB
		200000000, // 200MB
		16000000   // 16MB
	};
	
	Engine engine(info);
	engine.init();

	ResourceLoadError error;
	ResourcePack *pack = engine.get_resource_cache().add_resource<ResourcePack>(
			Directory("../core/resources/kon_primitives/", engine.get_allocator_dynamic()), "kon_primitives");
	pack->load_metadata(error);
	engine.get_resource_cache().add_resource_pack("kon_primitives");
	engine.get_resource_cache().load_metadata_group(pack->get_instance_id());
	engine.get_resource_cache().load_resource_group(pack->get_instance_id());

	KN_INSTRUMENT_CLOSE_FILE();

	KN_INSTRUMENT_NEW_FILE("logs/update.json");

	while(engine.update()) {
		engine.get_event_bus().emit_event<EventEngineExit>();
	}

	KN_INSTRUMENT_CLOSE_FILE();

	KN_INSTRUMENT_NEW_FILE("logs/close.json")

	engine.clean();

	KN_INSTRUMENT_CLOSE_FILE();

	KN_CORE_INFO("finished destroying everything :3");
}

