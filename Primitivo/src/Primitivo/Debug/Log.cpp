#include "plpch.h"
#include "Log.h"


#include <chrono>
#include <ctime>
#include <cstdarg>

//https://www.youtube.com/watch?v=l9e8PJskYnI&list=PLv8Ddw9K0JPg1BEO-RS-0MYs423cvLVtj&index=23&ab_channel=TravisVroman

namespace Primitivo {
	Log::Log() {
	}

	Log::~Log() {
	}

	void Log::Init() {
		
	}


	void Log::RegisterLog(bool checkCondition, const LogCategory& category, std::string message, ...) {
		

		std::string finalString = "";

		if (checkCondition == true) {
			char buffer[512];
			va_list args;

			va_start(args, message);
			vsprintf_s(buffer, message.c_str(), args);
			va_end(args);

			finalString = "[" + logCategoryList[category] + "] [" + CurrentDateTimeToString() + "] :: " + std::string(buffer);

			std::cout << finalString << std::endl;
		}

		//return "finalString";
	}

	std::string Log::CurrentDateTimeToString() {
		std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		std::string output(30, '\0');
		struct tm newtime;
		localtime_s(&newtime, &now);
		std::strftime(&output[0], output.size(), "%d-%b-%Y %H:%M:%S", &newtime);
		return output;
	}


}