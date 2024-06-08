#pragma once
#include "Taller/Core/Core.h"

#ifdef TL_PLATFORM_WINDOWS
extern Taller::Application* Taller::CreateApplication();
	
int main(int argc, char** argv) {
	TL_PROFILE_BEGIN_SESSION("Startup", "TallerProfile-Startup.json");
	auto app = Taller::CreateApplication();
	TL_PROFILE_END_SESSION();

	TL_PROFILE_BEGIN_SESSION("Runtime", "TallerProfile-Runtime.json");
	app->Run();
	TL_PROFILE_END_SESSION();
	
	TL_PROFILE_BEGIN_SESSION("Shutdown", "TallerProfile-Shutdown.json");
	delete app;
	TL_PROFILE_END_SESSION();

	return 0;
}
#else
	#error Taller at the moment only supports Windows!
#endif