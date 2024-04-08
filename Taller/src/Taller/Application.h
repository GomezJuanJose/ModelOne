#pragma once

#include "Core.h"
#include "Window.h"

#include "Taller/Events/Event.h"
#include "Taller/Events/ApplicationEvents.h"

#include "Taller/LayerStack.h"
#include "Taller/ImGui/ImGuiLayer.h"

namespace Taller {
	class Application {
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);


	private:
		bool m_IsRunning = true;
		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;
		std::unique_ptr<Window> m_Window;
		

	private:
		static Application* s_Instance;
	};

	//To be defined in client
	Application* CreateApplication();
}

