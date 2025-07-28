
#include "kon/core/assert.hpp"
#include "kon/debug/instrumentation.hpp"
#include "kon/debug/timer.hpp"
#include <chrono>
#include <kon/debug/log.hpp>

int ahh() {
	KN_INSTRUMENT_FUNCTION()

	KN_CORE_TRACE("log");
	return 0;
}

int main() {
	KN_INSTRUMENT_NEW_FILE("logs/startup.json");

	// KN_INSTRUMENT_FUNCTION();

	{
		KN_INSTRUMENT_SCOPE("time logs");
		kon::Timer timer;
		timer.start_timer();

		KN_CORE_TRACE("hello!");
		KN_ERROR("ahh");

		timer.end_timer();
	

		KN_CORE_INFO("time {}", timer.get_time<std::chrono::microseconds>());
	}
	
	KN_ASSERT(true, "oh no!");

	ahh();

	KN_INSTRUMENT_CLOSE_FILE();
}

