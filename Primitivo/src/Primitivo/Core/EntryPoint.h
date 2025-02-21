#pragma once
#include "Primitivo/Core/Core.h"

#ifdef TL_PLATFORM_WINDOWS
extern Primitivo::Application* Primitivo::CreateApplication();
	
int main(int argc, char** argv) {
	TL_PROFILE_BEGIN_SESSION("Startup", "PrimitivoProfile-Startup.json");
	auto app = Primitivo::CreateApplication();
	TL_PROFILE_END_SESSION();

	TL_PROFILE_BEGIN_SESSION("Runtime", "PrimitivoProfile-Runtime.json");
	app->Run();
	TL_PROFILE_END_SESSION();
	
	TL_PROFILE_BEGIN_SESSION("Shutdown", "PrimitivoProfile-Shutdown.json");
	delete app;
	TL_PROFILE_END_SESSION();

	return 0;
}
#else
	#error Primitivo at the moment only supports Windows!
#endif