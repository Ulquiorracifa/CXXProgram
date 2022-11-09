#include <vector>
#include <logger/logger.h>
namespace engine::logger
{
	extern void default_init(const std::string& path, const std::string& name, bool enable_console,bool async, int level, const std::string& pattern)
	{
		auto tmp_logger = spdlog::get(name);
		if (!tmp_logger)
		{
			std::vector <spdlog::sink_ptr> sinks;
			auto daily_sink = std::make_shared < spdlog::sinks::daily_file_format_sink_mt>(path + name + ".log", 23, 59);
			sinks.push_back(daily_sink);
			
			if (enable_console)
			{
				auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
				sinks.push_back(console_sink);
			}
			if (async)
			{
				auto thread_p = std::make_shared<spdlog::details::thread_pool>(8192, 1);
				tmp_logger = std::make_shared<spdlog::async_logger>(name, sinks.begin(), sinks.end(), thread_p, spdlog::async_overflow_policy::block);
			}
			else
			{
				tmp_logger = std::make_shared<spdlog::logger>(name, sinks.begin(), sinks.end());
			}
			if (level < 0 || level > spdlog::level::level_enum::off)
			{
				level = 0;
			}
			tmp_logger->flush_on(spdlog::level::level_enum::warn);
			tmp_logger->set_level((spdlog::level::level_enum)level);
			tmp_logger->set_pattern(pattern); //"[%Y-%m-%d.%e %H:%M:%S %z] [%n] [%^%l%$]%v"
			spdlog::set_default_logger(tmp_logger);
			spdlog::flush_every(std::chrono::seconds(1));
		}
	}
}


