#ifndef KN_LOG_HPP
#define KN_LOG_HPP

#include <spdlog/logger.h>
#include <memory>

namespace kon {

class Log {
private:
	Log();
	~Log();

public:
	std::shared_ptr<spdlog::logger> m_corelog;
	std::shared_ptr<spdlog::logger> m_clientlog;

public:
	static Log &get() {
		static Log l;
		return l;
	}
};

}

#define KN_CORE_TRACE(...) ::kon::Log::get().m_corelog->trace(__VA_ARGS__)
#define KN_CORE_INFO(...) ::kon::Log::get().m_corelog->info(__VA_ARGS__)
#define KN_CORE_WARN(...) ::kon::Log::get().m_corelog->warn(__VA_ARGS__)
#define KN_CORE_ERROR(...) ::kon::Log::get().m_corelog->error(__VA_ARGS__)

#define KN_TRACE(...) ::kon::Log::get().m_clientlog->trace(__VA_ARGS__)
#define KN_INFO(...) ::kon::Log::get().m_clientlog->info(__VA_ARGS__)
#define KN_WARN(...) ::kon::Log::get().m_clientlog->warn(__VA_ARGS__)
#define KN_ERROR(...) ::kon::Log::get().m_clientlog->error(__VA_ARGS__)

#endif
