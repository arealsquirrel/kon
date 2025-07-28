
#include "instrumentation.hpp"

namespace kon {

Instrumentor::Instrumentor() = default;
Instrumentor::~Instrumentor() = default;

void Instrumentor::write_header() {
	KN_ASSERT(m_file.is_open(), "Instrumentor::WriteHeader file is not open :3");
	m_file << "{\"otherData\": {},\"traceEvents\":[\n";
    m_file.flush();
}

void Instrumentor::write_footer() {
	KN_ASSERT(m_file.is_open(), "Instrumentor::WriteHeader file is not open :3");
	m_file << "\n]}";
	m_file.flush();
}

void Instrumentor::open_file(ShortString path) {
	m_file.open(path.c_str());
	m_profileCount = 0;
	write_header();
	m_fileStartTime = std::chrono::steady_clock::now();
}

void Instrumentor::write_debug_frame(DebugFrame frame) {
	KN_ASSERT(m_file.is_open(), "Instrumentor::WriteHeader file is not open :3");

	// std::lock_guard lock(mutex);

	if(m_profileCount++ > 0)
		m_file << ',';

	// std::string name = frame.name;
	// std::replace(name.begin(), name.end(), '"', '\'');

	m_file << "{";
	m_file << "\"cat\":\"function\",";
	m_file << "\"dur\":" << (frame.duration) << ',';
	m_file << "\"name\":\"" << frame.name.c_str() << "\",";
	m_file << "\"ph\":\"X\",";
	m_file << "\"pid\":0,";
	m_file << "\"tid\":" << frame.threadID << ",";
	m_file << "\"ts\":" << std::chrono::duration_cast<std::chrono::microseconds>(frame.startTime - m_fileStartTime).count();
	m_file << "}\n";
	
	m_file.flush();

	// mutex.unlock();
}

void Instrumentor::close_file() {
	write_footer();
	m_file.close();
}

InstrumentorMeasure::InstrumentorMeasure(ShortString name, u32 threadID) 
	: m_name(name), m_timer(), m_threadID(threadID) {

	m_timer.start_timer();
}

InstrumentorMeasure::~InstrumentorMeasure() {
	m_timer.end_timer();
	
	Instrumentor::Get().write_debug_frame(DebugFrame{
				m_timer.get_time<std::chrono::microseconds>(),
				m_threadID,
				m_timer.get_start(),
				m_name
			});
}

}

