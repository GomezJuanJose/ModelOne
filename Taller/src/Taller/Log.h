#pragma once

#include "Core.h"
#include <string>
#include <vector>

// TODO: Implement loggin basics. Needs to print a message with a category, n argumenst with x color and check if prints it given a bool.
// TODO: Strip the logs when is distribution build

namespace Taller {
	enum LogCategory {
		FATAL,
		ERROR,
		WARNING,
		INFO,
		SUSCCESS
	};


	class TALLER_API Log {
	public:

		Log();
		~Log();

		static void Init();

		static void RegisterLog(bool checkCondition, const LogCategory& category, std::string message, ...);

	private:
		static std::string CurrentDateTimeToString();

	private:
		inline static const std::string logCategoryList[] = { "FATAL", "ERROR", "WARNING", "INFO", "SUSCCESS" }; //TODO Do it more flexible and efficient

	};
}

//TODO Disable INFO and WARNING logs when is not debug
//TODO Fix the error relative of the macro which allows anyway to compile
#define TL_LOG_ERROR(checkCondition, message, ...) ::Taller::Log::RegisterLog(checkCondition, Taller::ERROR, message, __VA_ARGS__)
#define TL_LOG_WARNING(checkCondition, message, ...) ::Taller::Log::RegisterLog(checkCondition, Taller::WARNING, message, __VA_ARGS__)
#define TL_LOG_INFO(checkCondition, message, ...) ::Taller::Log::RegisterLog(checkCondition, Taller::INFO, message, __VA_ARGS__)
#define TL_LOG_SUSCCESS(checkCondition, message, ...) ::Taller::Log::RegisterLog(checkCondition, Taller::SUSCCESS, message, __VA_ARGS__)
