#pragma once

#include "Core.h"
#include "Window.h"

#include "Taller/Events/ApplicationEvents.h"


namespace Taller {
	class TALLER_API Application {
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

	private:
		bool OnWindowClose(WindowCloseEvent& e);


	private:
		bool m_IsRunning = true;
		
		std::unique_ptr<Window> m_Window;
	};

	//To be defined in client
	Application* CreateApplication();
}

