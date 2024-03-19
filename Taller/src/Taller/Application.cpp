#include "tlpch.h"
#include "Application.h"

namespace Taller {
	Taller::Application::Application() {
		m_Window = std::unique_ptr<Window>(Window::Create());
	}

	Taller::Application::~Application() {
	}

	void Application::Run() {
		while (m_IsRunning) {
			m_Window->OnUpdate();
		}
	}
}
