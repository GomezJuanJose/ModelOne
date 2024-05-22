#include <Taller.h>

#include "ImGui/imgui.h"

class ExampleLayer : public Taller::Layer {
public:
	ExampleLayer() : Layer("Example") {
		m_TriangleVA.reset(Taller::VertexArray::Create());

		float triangleVertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		std::shared_ptr<Taller::VertexBuffer> triangleVB;
		triangleVB.reset(Taller::VertexBuffer::Create(triangleVertices, sizeof(triangleVertices)));
		Taller::BufferLayout triangleLayout = {
			{Taller::ShaderDataType::Float3, "a_Position"},
			{Taller::ShaderDataType::Float4, "a_Color"}
		};
		triangleVB->SetLayout(triangleLayout);
		m_TriangleVA->AddVertexBuffer(triangleVB);

		uint32_t triangleIndices[3] = { 0, 1, 2 };
		std::shared_ptr<Taller::IndexBuffer> triangleIB;
		triangleIB.reset(Taller::IndexBuffer::Create(triangleIndices, sizeof(triangleIndices) / sizeof(uint32_t)));
		m_TriangleVA->SetIndexBuffer(triangleIB);

		
		m_SquareVA.reset(Taller::VertexArray::Create());

		float squareVertices[4 * 3] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};

		std::shared_ptr<Taller::VertexBuffer> squareVB;
		squareVB.reset(Taller::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		Taller::BufferLayout squareLayout = {
			{Taller::ShaderDataType::Float3, "a_Position"}
		};
		squareVB->SetLayout(squareLayout);
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Taller::IndexBuffer> squareIB;
		squareIB.reset(Taller::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);


		std::string triangleVertexSrc = R"(

			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
		
			uniform mat4 u_ProjectionViewMatrix;
			uniform mat4 u_ModelMatrix;

			out vec3 v_Position;
			out vec4 v_Color;

			void main(){
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ProjectionViewMatrix * u_ModelMatrix * vec4(v_Position, 1.0);
			}
		)";


		std::string triangleFragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;
		
			in vec3 v_Position;
			in vec4 v_Color;

			void main(){
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		m_TriangleShader.reset(new Taller::Shader(triangleVertexSrc, triangleFragmentSrc));


		std::string squareVertexSrc = R"(

			#version 330 core

			layout(location = 0) in vec3 a_Position;
			
			uniform mat4 u_ProjectionViewMatrix;
			uniform mat4 u_ModelMatrix;
		
			out vec3 v_Position;


			void main(){
				v_Position = a_Position;
				gl_Position = u_ProjectionViewMatrix * u_ModelMatrix * vec4(v_Position, 1.0);
			}
		)";


		std::string squareFragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;
		
			in vec3 v_Position;


			void main(){
				color = vec4(0.8, 0.2, 0.3, 1.0);

			}
		)";

		m_SquareShader.reset(new Taller::Shader(squareVertexSrc, squareFragmentSrc));


		Taller::Entity camera = coord.CreateEntity();
		coord.AddComponent<Taller::CameraComponent>(camera, 45.0f,1.7f, 0.1f, 100.0f);
		coord.AddComponent<Taller::TransformComponent>(camera);
		coord.GroupEntity(camera, "cam");

		cam = coord.GetComponent<Taller::CameraComponent>(camera);
		transform = coord.GetComponent<Taller::TransformComponent>(camera);
		transform.location = glm::vec3(0.0f, 0.0f, 3.0f);
		transform.rotation = glm::vec3(0.0f, 0.0f, 0.0f);

		Taller::ComponentOperations::CalculateCameraViewProjection(cam, transform);

	};

	void OnUpdate() override {
		Taller::RenderCommand::SetClearColor({ 1.0f, 0.0f, 1.0f, 1.0f });
		Taller::RenderCommand::Clear();
		
		Taller::Renderer::BeginScene(cam.viewProjection);

			Taller::Renderer::Submit(m_SquareShader, m_SquareVA, glm::vec3(0.0f), glm::vec3(-55.0f, 10.0f, 45.0f));
			Taller::Renderer::Submit(m_TriangleShader, m_TriangleVA);

		Taller::Renderer::EndScene();
	}

	void OnEvent(Taller::Event& e) override {
		TL_LOG_INFO(true, "Layer event : %s", e.ToString());
	}

	virtual void OnImGuiRender() override {
		static bool show = true;
		ImGui::ShowDemoWindow(&show);
	}


private:
	std::shared_ptr<Taller::Shader> m_TriangleShader;
	std::shared_ptr<Taller::VertexArray> m_TriangleVA;
	
	std::shared_ptr<Taller::Shader> m_SquareShader;
	std::shared_ptr<Taller::VertexArray> m_SquareVA;

	Taller::TransformComponent& transform = Taller::TransformComponent();
	Taller::CameraComponent& cam = Taller::CameraComponent();


	Taller::Coordinator& coord = Taller::Application::Get().GetCoordinator();
};

class Sandbox : public Taller::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
	}
	~Sandbox() {}

private:

};

Taller::Application* Taller::CreateApplication() {
	return new Sandbox();
}