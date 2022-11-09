#pragma once
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/async.h>
#include <utils/platform.hpp>
namespace engine::logger
{
	EngineAPI void default_init(const std::string& path, const std::string& name, bool enable_console, bool async, int level, const std::string& pattern);
};
#define Log_Trace(...)	spdlog::trace(__VA_ARGS__)
#define Log_Debug(...)	spdlog::debug(__VA_ARGS__)
#define Log_Info(...)	spdlog::info(__VA_ARGS__)
#define Log_Warn(...)	spdlog::warn(__VA_ARGS__)
#define Log_Error(...)	spdlog::error(__VA_ARGS__)
#define Log_Critical(...)	spdlog::critical(__VA_ARGS__)

