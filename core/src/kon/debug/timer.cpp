
#include "timer.hpp"
#include <chrono>

namespace kon {

Timer::Timer() = default;

Timer::~Timer() = default;

void Timer::start_timer() {
	m_start = std::chrono::steady_clock::now();
}

void Timer::end_timer() {
	m_end = std::chrono::steady_clock::now();
}

}

