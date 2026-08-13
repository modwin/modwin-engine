//
// Created by komvu on 2024-12-10.
//

#ifndef MODWIN_ENGINE_LOG_H
#define MODWIN_ENGINE_LOG_H

#include <memory>
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

