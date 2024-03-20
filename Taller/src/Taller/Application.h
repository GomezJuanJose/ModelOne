#pragma once

#include "Core.h"
#include "Window.h"

#include "Taller/Events/Event.h"
#include "Taller/Events/ApplicationEvents.h"

#include "Taller/LayerStack.h"

namespace Taller {
	class TALLER_API Application {
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

	private:
		bool OnWindowClose(WindowCloseEvent& e);


	private:
		bool m_IsRunning = true;
		LayerStack m_LayerStack;
		std::unique_ptr<Window> m_Window;
	};

	//To be defined in client
	Application* CreateApplication();
}

