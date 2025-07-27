#ifndef KN_TIMER_HPP
#define KN_TIMER_HPP

#include <chrono>
#include <kon/core/core.hpp>

namespace kon {

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

private:
	std::chrono::time_point<std::chrono::steady_clock> m_start;
	std::chrono::time_point<std::chrono::steady_clock> m_end;
	
	bool ended;
};

}

#endif
