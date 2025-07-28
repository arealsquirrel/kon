
#include "instrumentation.hpp"

namespace kon {

Instrumentor::Instrumentor() = default;
Instrumentor::~Instrumentor() = default;

void Instrumentor::write_header() {
	KN_ASSERT(file.is_open(), "Instrumentor::WriteHeader file is not open :3");
	file << "{\"otherData\": {},\"traceEvents\":[\n";
    file.flush();
}

void Instrumentor::write_footer() {
	KN_ASSERT(file.is_open(), "Instrumentor::WriteHeader file is not open :3");
		file << "\n]}";
        file.flush();
}

void open_file(ShortString path);
void write_debug_frame(DebugFrame frame);
void close_file();


InstrumentorMeasure::InstrumentorMeasure(ShortString name, size_t threadID) {

}

InstrumentorMeasure::~InstrumentorMeasure() {

}

void InstrumentorMeasure::end_timer() {

}

}

