//
// Created by komvu on 2024-12-10.
//

#include "Log.h"
#include <spdlog/spdlog.h>
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/ansicolor_sink.h"
#include <string>


namespace Winther
{

	Log::Log() = default;

	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;


	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v %$");
		s_CoreLogger = spdlog::stdout_color_mt("ENGINE");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("CLIENT");
		s_ClientLogger->set_level(spdlog::level::trace);

		s_ClientLogger->info("Log initialized.");
		s_CoreLogger->info("Log initialized.");
	}

	Log::~Log()
	{
		s_CoreLogger = nullptr;
		s_ClientLogger = nullptr;
		delete this;
	}

}
