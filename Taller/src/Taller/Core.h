#pragma once

#ifdef TL_PLATFORM_WINDOWS
	#ifdef TL_BUILD_DLL
		#define TALLER_API __declspec(dllexport)
	#else
		#define TALLER_API __declspec(dllimport)
	#endif
#else
	#error Taller at the moment only supports Windows!
#endif
