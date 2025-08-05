
#include "log.hpp"

#ifdef KN_ENABLE_LOGGING

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

namespace kon {

Log::Log() {
	std::vector<spdlog::sink_ptr> logSinks;
	logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
	logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/kon.log", true));

	logSinks[0]->set_pattern("%^[%T %n] %v%$");
	logSinks[1]->set_pattern("[%T] [%l] %n: %v");

	m_corelog = std::make_shared<spdlog::logger>("core", begin(logSinks), end(logSinks));
	spdlog::register_logger(m_corelog);
	m_corelog->set_level(spdlog::level::trace);
	m_corelog->flush_on(spdlog::level::trace);

	m_clientlog = std::make_shared<spdlog::logger>("client", begin(logSinks), end(logSinks));
	spdlog::register_logger(m_clientlog);
	m_clientlog->set_level(spdlog::level::trace);
	m_clientlog->flush_on(spdlog::level::trace);
}

Log::~Log() = default;

}

#endif
