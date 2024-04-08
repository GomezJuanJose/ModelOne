#pragma once

/*#ifdef TL_PLATFORM_WINDOWS
	#ifdef TL_BUILD_DLL
		#define TALLER_API __declspec(dllexport)
	#else
		#define TALLER_API __declspec(dllimport)
	#endif
#else
	#error Taller at the moment only supports Windows!
#endif
*/

//TODO Create a own class of assertions
#ifdef TL_ENABLE_ASSERTS
	#define TL_ASSERT(condition, message, ...) {if(!(condition)){TL_LOG_ERROR(true, "Assertion failed! => "+message, __VA_ARGS__); __debugbreak();}}
#else
	#define TL_ASSERT(condition, message, ...)
#endif

#define BIT(x) (1 << x)