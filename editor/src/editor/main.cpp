
#include "kon/core/assert.hpp"
#include "kon/debug/timer.hpp"
#include <chrono>
#include <kon/debug/log.hpp>

int main() {
	kon::Timer timer;
	timer.start_timer();

	KN_CORE_TRACE("hello!");
	KN_ERROR("ahh");

	timer.end_timer();

	KN_CORE_INFO("time {}", timer.get_time<std::chrono::microseconds>());

	KN_ASSERT(true, "oh no!");
}

