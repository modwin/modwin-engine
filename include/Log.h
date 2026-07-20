//
// Created by komvu on 2024-12-10.
//

#ifndef WINTHER_ENGINE_LOG_H
#define WINTHER_ENGINE_LOG_H

#include "core/Engine.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <spdlog/logger.h>

namespace Modwin
{

	class Log
	{
	public:
		Log();

		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger();

		inline static std::shared_ptr<spdlog::logger>& GetClientLogger();

		~Log();

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;

	};

	std::shared_ptr<spdlog::logger>& Log::GetCoreLogger()
	{
		return s_CoreLogger;
	}

	std::shared_ptr<spdlog::logger>& Log::GetClientLogger()
	{
		return s_ClientLogger;
	}


} // Modwin

#endif

