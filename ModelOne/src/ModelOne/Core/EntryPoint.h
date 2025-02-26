#pragma once
#include "ModelOne/Core/Core.h"

#ifdef MO_PLATFORM_WINDOWS
extern ModelOne::Application* ModelOne::CreateApplication();
	
int main(int argc, char** argv) {
	MO_PROFILE_BEGIN_SESSION("Startup", "ModelOneProfile-Startup.json");
	auto app = ModelOne::CreateApplication();
	MO_PROFILE_END_SESSION();

	MO_PROFILE_BEGIN_SESSION("Runtime", "ModelOneProfile-Runtime.json");
	app->Run();
	MO_PROFILE_END_SESSION();
	
	MO_PROFILE_BEGIN_SESSION("Shutdown", "ModelOneProfile-Shutdown.json");
	delete app;
	MO_PROFILE_END_SESSION();

	return 0;
}
#else
	#error ModelOne at the moment only supports Windows!
#endif