
#include "Log.h"

#include <iostream>
#include <chrono>
#include <ctime>
#include <cstdarg>

//https://www.youtube.com/watch?v=l9e8PJskYnI&list=PLv8Ddw9K0JPg1BEO-RS-0MYs423cvLVtj&index=23&ab_channel=TravisVroman

namespace Taller {
	Log::Log() {
	}

	Log::~Log() {
	}

	void Log::Init() {
		//TODO Init log file
		//TODO Init load custom categories and colors?
	}

	//TODO Add color
	void Log::RegisterLog(bool checkCondition, const LogCategory& category, std::string message, ...) {
		//TODO Async print
		//TODO Make efficient printing

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

	//TODO Assertions
}