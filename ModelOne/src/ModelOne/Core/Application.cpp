#include "mopch.h"
#include "Application.h"
#include "ModelOne/Renderer/Renderer.h"

#include "GLFW/glfw3.h" //TODO for a feature remove this and move the timestep

#include "Window.h"

#include "ModelOne/Events/Event.h"
#include "ModelOne/Events/ApplicationEvents.h"

#include "ModelOne/ECS/ECS.h"

#include "ModelOne/Core/Timestep.h"

#include "ModelOne/Renderer/RenderLayer.h"
#include "ModelOne/Physics/PhysicsLayer.h"
#include "ModelOne/Audio/AudioLayer.h"
#include "ModelOne/ImGui/ImGuiLayer.h"

#include "ModelOne/Renderer/MeshLibrary.h"
#include "ModelOne/Renderer/Texture2DLibrary.h"
#include "ModelOne/Renderer/ShaderLibrary.h"
#include "ModelOne/Audio/SoundLibrary.h"


namespace ModelOne {


#define BIND_EVENT_FUNCTION(f) std::bind(&Application::f, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;
	
	Application::Application() {

		MO_PROFILE_FUNCTION();


		MO_ASSERT(!s_Instance, "Application already exists!");
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

		MO_PROFILE_FUNCTION();

	}

	void Application::Run() {

		MO_PROFILE_FUNCTION();

		while (m_IsRunning) {

			float time = glfwGetTime(); //TODO Search for "Fix Your Time Step"
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_IsMinimize) {
				{
					MO_PROFILE_SCOPE("LayerStack->OnUpdate");

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

		MO_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FUNCTION(OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FUNCTION(OnWindowResize));

		MO_LOG_INFO(true, e.ToString());

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

		MO_PROFILE_FUNCTION();

		m_IsMinimize = false;

		if (e.GetWidth() == 0.0f || e.GetHeight() == 0.0f) {
			m_IsMinimize = true;
		}

		
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}

	void Application::PushLayer(Layer* layer) {

		MO_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay) {

		MO_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}
}
