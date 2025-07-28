#ifndef SC_INSTRUMENTATION_HPP
#define SC_INSTRUMENTATION_HPP

#include "kon/container/string.hpp"
#include <fstream>
#include <kon/core/core.hpp>
#include <chrono>
#include <kon/debug/timer.hpp>


/*
 * this code was taken from an erlier game engine that i made,
 * and so this might seem very similar to the cherno's instrumentor
 * im just notifiying yall because he was BIG help
 */
namespace kon {

struct DebugFrame
{
	i64 duration; // duration in microseconds
	u32 threadID;
	TimePoint startTime;
	ShortString name;
};

class Instrumentor
{
public:
	Instrumentor(const Instrumentor&) = delete;

	void open_file(ShortString path);
	void write_debug_frame(DebugFrame frame);
	void close_file();

	static Instrumentor &Get(){
		static Instrumentor i;
		return i;
	}

private:
	Instrumentor();
	~Instrumentor();

	void write_header();
	void write_footer();

private:
	std::ofstream m_file;
	int m_profileCount {0};
	TimePoint m_fileStartTime;
};

struct InstrumentorMeasure {
public:
	InstrumentorMeasure(ShortString name, u32 threadID=0);
	~InstrumentorMeasure();

private:
	ShortString m_name { "_null" };
	Timer m_timer;
	u32 m_threadID;
};

#define KN_INSTRUMENT_TIMER(tname, ...) ::kon::InstrumentorMeasure tname(__VA_ARGS__);
#define KN_INSTRUMENT_NEW_FILE(name) ::kon::Instrumentor::Get().open_file(name);
#define KN_INSTRUMENT_CLOSE_FILE() ::kon::Instrumentor::Get().close_file();

#define KN_INSTRUMENT_SCOPE(...) KN_INSTRUMENT_TIMER(kn_timer, __VA_ARGS__)
#define KN_INSTRUMENT_FUNCTION() KN_INSTRUMENT_TIMER(kn_function, __func__, 0)

}

#endif
