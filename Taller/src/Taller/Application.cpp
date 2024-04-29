#include "tlpch.h"
#include "Application.h"
#include <glad/glad.h>



//Explicar el porque de la excepcion de los dos heaps distintos cuando se accede desde un dll

namespace Taller {

#define BIND_EVENT_FUNCTION(f) std::bind(&Application::f, this, std::placeholders::_1) //TODO Make it more niecer depending of how the event system evolves

	Application* Application::s_Instance = nullptr;
	
	Application::Application() {
		TL_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FUNCTION(OnEvent));

		m_Coordinator = std::make_unique<Coordinator>();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);	
	}

	Application::~Application() {
	}

	void Application::Run() {
		while (m_IsRunning) {

			glClearColor(0, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : m_LayerStack) {
				layer->OnUpdate();
			}


			m_ImGuiLayer->BeginImGuiRender();
			for (Layer* layer : m_LayerStack) {
				layer->OnImGuiRender();
			}
			m_ImGuiLayer->EndImGuiRender();


			m_Window->OnUpdate();
			m_Coordinator->Update();
		}
	}

	void Application::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FUNCTION(OnWindowClose));

		TL_LOG_INFO(true, e.ToString());

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->OnEvent(e);
			if (e.Handled) {
				break;
			}
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e) {
		m_IsRunning = false;
		return true;
	}

	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay) {
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}
}
