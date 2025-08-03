
#include "kon/core/events.hpp"
#include "kon/engine/engine.hpp"
#include "kon/resource/resource.hpp"
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
	
	{
		Engine engine(info);
		engine.init();

		KN_INSTRUMENT_CLOSE_FILE();

		KN_INSTRUMENT_NEW_FILE("logs/update.json");

		while(engine.update()) {
			engine.get_event_bus().emit_event<EventEngineExit>();
		}

		KN_INSTRUMENT_CLOSE_FILE();

		KN_INSTRUMENT_NEW_FILE("logs/close.json")

		engine.clean();
	} // get the engine out of scope so we can log its shutdown

	KN_INSTRUMENT_CLOSE_FILE();
}

