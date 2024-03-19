#pragma once

#include "Core.h"
#include "Window.h"

namespace Taller {
	class TALLER_API Application {
		public:
			Application();
			virtual ~Application();

			void Run();

	private:
		bool m_IsRunning = true;
		
		std::unique_ptr<Window> m_Window;


	};

	//To be defined in client
	Application* CreateApplication();
}

