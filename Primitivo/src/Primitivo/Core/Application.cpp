#include "plpch.h"
#include "Application.h"
#include "Primitivo/Renderer/Renderer.h"

#include "GLFW/glfw3.h" //TODO for a feature remove this and move the timestep

#include "Window.h"

#include "Primitivo/Events/Event.h"
#include "Primitivo/Events/ApplicationEvents.h"

#include "Primitivo/ECS/ECS.h"

#include "Primitivo/Core/Timestep.h"

#include "Primitivo/Renderer/RenderLayer.h"
#include "Primitivo/Physics/PhysicsLayer.h"
#include "Primitivo/Audio/AudioLayer.h"
#include "Primitivo/ImGui/ImGuiLayer.h"

#include "Primitivo/Renderer/MeshLibrary.h"
#include "Primitivo/Renderer/Texture2DLibrary.h"
#include "Primitivo/Renderer/ShaderLibrary.h"
#include "Primitivo/Audio/SoundLibrary.h"


namespace Primitivo {


#define BIND_EVENT_FUNCTION(f) std::bind(&Application::f, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;
	
	Application::Application() {

		TL_PROFILE_FUNCTION();


		TL_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FUNCTION(OnEvent));

		m_Coordinator = std::make_unique<Coordinator>();
		m_SoundLibrary = std::make_unique<SoundLibrary>();
		m_ShaderLibrary = std::make_unique<ShaderLibrary>();
		m_MeshLibrary = std::make_unique<MeshLibrary>();
		m_Texture2DLibrary = std::make_unique<Texture2DLibrary>();

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);	

		m_RenderLayer = new RenderLayer();
		PushLayer(m_RenderLayer);

		m_PhysicsLayer = new PhysicsLayer();
		PushLayer(m_PhysicsLayer);

		m_AudioLayer = new AudioLayer();
		PushLayer(m_AudioLayer);
	}

	Application::~Application() {

		TL_PROFILE_FUNCTION();

	}

	void Application::Run() {

		TL_PROFILE_FUNCTION();

		while (m_IsRunning) {

			float time = glfwGetTime(); //TODO Search for "Fix Your Time Step"
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_IsMinimize) {
				{
					TL_PROFILE_SCOPE("LayerStack->OnUpdate");

					for (Layer* layer : m_LayerStack) {
						layer->OnUpdate(timestep);
					}
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

		TL_PROFILE_FUNCTION();

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

		TL_PROFILE_FUNCTION();

		m_IsMinimize = false;

		if (e.GetWidth() == 0.0f || e.GetHeight() == 0.0f) {
			m_IsMinimize = true;
		}

		
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}

	void Application::PushLayer(Layer* layer) {

		TL_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay) {

		TL_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}
}
