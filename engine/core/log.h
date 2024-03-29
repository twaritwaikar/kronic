#pragma once

#include "common.h"
#include "os/os.h"

#include "spdlog/spdlog.h"
#include "spdlog/logger.h"
#include "spdlog/pattern_formatter.h"
#include "spdlog/sinks/ostream_sink.h"

struct Log
{
	template <typename... Args>
	static void info(Args&&... args)
	{
		spdlog::info(std::forward<Args>(args)...);
	}

	template <typename... Args>
	static void warn(Args&&... args)
	{
		spdlog::warn(std::forward<Args>(args)...);
	}

	template <typename... Args>
	static void error(Args&&... args)
	{
		spdlog::error(std::forward<Args>(args)...);
	}

	template <typename... Args>
	static void debug(Args&&... args)
	{
		spdlog::debug(std::forward<Args>(args)...);
	}

	// https://github.com/gabime/spdlog/blob/f5492aed12b144df73faf8ae336b3dbc39ebbc38/tests/test_pattern_formatter.cpp#L7
	template <typename... Args>
	static std::string log_to_str(Args... args)
	{
		std::ostringstream oss;
		auto oss_sink = std::make_shared<spdlog::sinks::ostream_sink_mt>(oss);
		spdlog::logger oss_logger("pattern", oss_sink);

		oss_logger.set_level(spdlog::level::critical);
		oss_logger.set_formatter(MakeUnique<spdlog::pattern_formatter>("%v", spdlog::pattern_time_type::local));

		oss_logger.critical(std::forward<Args>(args)...);
		return oss.str();
	}

	template <typename... Args>
	static void critical(Args&&... args)
	{
		String error_msg = log_to_str(std::forward<Args>(args)...);

		spdlog::critical(std::forward<Args>(args)...);
		OS::get_singleton()->post_error_message("Kritical Error", error_msg);
		OS::get_singleton()->crash();
	}

	static void setup()
	{

#ifndef NDEBUG
		spdlog::set_level(spdlog::level::trace);
#endif // NDEBUG
	}
};

#define LOG_FORMAT " \033[90m({}#L{})\033[39m"

#define DEBUG(fmt, ...) Log::debug(fmt LOG_FORMAT, __VA_ARGS__, __func__, __LINE__)
#define INFO(fmt, ...) Log::info(fmt LOG_FORMAT, __VA_ARGS__, __func__, __LINE__)
#define ERR(fmt, ...) Log::error(fmt LOG_FORMAT, __VA_ARGS__, __func__, __LINE__)
#define WARN(fmt, ...) Log::warn(fmt LOG_FORMAT, __VA_ARGS__, __func__, __LINE__)
#define CRITICAL(fmt, ...) Log::critical(fmt LOG_FORMAT, __VA_ARGS__, __func__, __LINE__)
