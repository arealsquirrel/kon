#ifndef KN_TIMER_HPP
#define KN_TIMER_HPP

#include <chrono>
#include <kon/core/core.hpp>

namespace kon {

using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;

/*
 * you have to call the functions to start and stop
 * the timer, it is not one of those object lifetime thingy's
 */
class Timer {
public:
	Timer();
	~Timer();

public:
	void start_timer();
	void end_timer();

	template<class T>
	long get_time() {
		return std::chrono::duration_cast<T>(
			m_end - m_start).count();
	}

	TimePoint get_start() const { return m_start; }
	TimePoint get_end() const { return m_end; }

private:
	TimePoint m_start;
	TimePoint m_end;
	
	bool ended;
};

}

#endif
