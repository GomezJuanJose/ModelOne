#pragma once

#include "ModelOne/Core/Core.h"


// TODO: Implement loggin basics. Needs to print a message with a category, n argumenst with x color and check if prints it given a bool.
// TODO: Strip the logs when is distribution build

namespace ModelOne {
	enum LogCategory {
		FATAL,
		ERROR,
		WARNING,
		INFO,
		SUSCCESS
	};


	class Log {
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


#define MO_LOG_ERROR(checkCondition, message, ...) ::ModelOne::Log::RegisterLog(checkCondition, ModelOne::ERROR, message, __VA_ARGS__)
#define MO_LOG_WARNING(checkCondition, message, ...) ::ModelOne::Log::RegisterLog(checkCondition, ModelOne::WARNING, message, __VA_ARGS__)
#define MO_LOG_INFO(checkCondition, message, ...) ::ModelOne::Log::RegisterLog(checkCondition, ModelOne::INFO, message, __VA_ARGS__)
#define MO_LOG_SUSCCESS(checkCondition, message, ...) ::ModelOne::Log::RegisterLog(checkCondition, ModelOne::SUSCCESS, message, __VA_ARGS__)
