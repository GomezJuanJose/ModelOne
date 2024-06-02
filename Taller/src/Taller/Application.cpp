#include "tlpch.h"
#include "Application.h"
#include "Taller/Renderer/Renderer.h"

#include "GLFW/glfw3.h" //TODO fix timestep and remove this!!!

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

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);	
	}

	Application::~Application() {
	}

	void Application::Run() {
		while (m_IsRunning) {

			float time = glfwGetTime(); //TODO Search for "Fix Your Time Step"
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_IsMinimize) {
				for (Layer* layer : m_LayerStack) {
					layer->OnUpdate(timestep);
				}
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
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FUNCTION(OnWindowResize));

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

	bool Application::OnWindowResize(WindowResizeEvent& e) {
		
		if (e.GetWidth() == 0.0f || e.GetHeight() == 0.0f) {
			m_IsMinimize = true;
		}

		m_IsMinimize = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
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
