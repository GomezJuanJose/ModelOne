#include "tlpch.h"
#include "Application.h"


//std::place_holder
//std::bind
//std::function<void(Event&)>

namespace Taller {

#define BIND_EVENT_FUNCTION(f) std::bind(&Application::f, this, std::placeholders::_1) //TODO Make it more niecer depending of how the event system evolves

	Application::Application() {
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FUNCTION(OnEvent));
	}

	Application::~Application() {
	}

	void Application::Run() {
		while (m_IsRunning) {
			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FUNCTION(OnWindowClose));

		TL_LOG_INFO(true, e.ToString());
	}

	bool Application::OnWindowClose(WindowCloseEvent& e) {
		m_IsRunning = false;
		return true;
	}
}
