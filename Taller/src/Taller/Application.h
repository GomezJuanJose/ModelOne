#pragma once

#include "Core.h"
#include "Window.h"

#include "Taller/Events/Event.h"
#include "Taller/Events/ApplicationEvents.h"

#include "Taller/LayerStack.h"
#include "Taller/ImGui/ImGuiLayer.h"

#include "Taller/ECS/ECS.h"
#include "Taller/ECS/Components.h"

#include "Taller/Core/Timestep.h"

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
		inline Coordinator& GetCoordinator() { return *m_Coordinator; }
		inline static Application& Get() { return *s_Instance; }
		

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);


	private:
		bool m_IsRunning = true;
		bool m_IsMinimize = false;

		float m_LastFrameTime = 0.0f;

		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;
		std::unique_ptr<Window> m_Window;
		std::unique_ptr<Coordinator> m_Coordinator;

	private:
		static Application* s_Instance;

	};

	//To be defined in client
	Application* CreateApplication();
}

