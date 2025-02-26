#pragma once

#include "Core.h"
#include "Window.h"


#include "ModelOne/Core/LayerStack.h"



namespace ModelOne {

	class Coordinator;
	class Event;
	class WindowCloseEvent;
	class WindowResizeEvent;

	class RenderLayer;
	class PhysicsLayer;
	class AudioLayer;
	class ImGuiLayer;

	class MeshLibrary;
	class Texture2DLibrary;
	class ShaderLibrary;
	class SoundLibrary;

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
		inline SoundLibrary& GetAudioLibrary() { return *m_SoundLibrary; }
		inline ShaderLibrary& GetShaderLibrary() { return *m_ShaderLibrary; }
		inline MeshLibrary& GetMeshLibrary() { return *m_MeshLibrary; }
		inline Texture2DLibrary& GetTextureLibrary() { return *m_Texture2DLibrary; }
		
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
		RenderLayer* m_RenderLayer;
		PhysicsLayer* m_PhysicsLayer;
		AudioLayer* m_AudioLayer;

		std::unique_ptr<Window> m_Window;
		std::unique_ptr<Coordinator> m_Coordinator;

		std::unique_ptr <SoundLibrary> m_SoundLibrary;
		std::unique_ptr <ShaderLibrary> m_ShaderLibrary;
		std::unique_ptr <MeshLibrary> m_MeshLibrary;
		std::unique_ptr <Texture2DLibrary> m_Texture2DLibrary;

	private:
		static Application* s_Instance;


	};

	//To be defined in client
	Application* CreateApplication();
}

