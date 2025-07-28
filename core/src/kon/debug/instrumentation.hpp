#ifndef SC_INSTRUMENTATION_HPP
#define SC_INSTRUMENTATION_HPP

#include "kon/container/string.hpp"
#include <fstream>
#include <kon/core/core.hpp>
#include <chrono>


/*
 * this code was taken from an erlier game engine that i made,
 * and so this might seem very similar to the cherno's instrumentor
 * im just notifiying yall because he was BIG help
 */
namespace kon {

using timePoint = std::chrono::time_point<std::chrono::system_clock>;

struct DebugFrame
{
	u32 duration; // duration in microseconds
	size_t threadID;
	timePoint startTime;
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
	std::ofstream file;
	std::mutex mutex;
	int profileCount {0};
	timePoint fileStartTime;
};

struct InstrumentorMeasure {
public:
	InstrumentorMeasure(ShortString name, size_t threadID=0);
	~InstrumentorMeasure();

	void end_timer();

private:
	ShortString programName { "_null" };
	timePoint start;
	timePoint end;
	bool running {true};
	u32 m_threadID;
};

#define SC_INSTRUMENT_TIMER(...) ::snowcrash::Timer sc_timer(__VA_ARGS__);
#define SC_INSTRUMENT_NEW_FILE(name) ::snowcrash::Instrumentor::Get().OpenFile(name);
#define SC_INSTRUMENT_CLOSE_FILE() ::snowcrash::Instrumentor::Get().CloseFile();

// #define SC_INSTRUMENT_FUNCTION(func, threadID) SC_INSTRUMENT_TIMER(__func__, threadID)
#define SC_INSTRUMENT_SCOPE(...) SC_INSTRUMENT_TIMER(__VA_ARGS__)
#define SC_INSTRUMENT_FUNCTION() SC_INSTRUMENT_TIMER(__func__, 0)

}

#endif
