#pragma once

#ifdef TL_PLATFORM_WINDOWS
extern Taller::Application* Taller::CreateApplication();
	
int main(int argc, char** argv) {
	auto app = Taller::CreateApplication();
	app->Run();
	delete app;

	return 0;
}
#else
	#error Taller at the moment only supports Windows!
#endif