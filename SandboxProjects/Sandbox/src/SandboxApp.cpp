#include <Taller.h>
#include <Taller/Core/EntryPoint.h>

#include "ImGui/imgui.h"

#pragma region PLACE_HOLDER_DATA
std::string triangleVertexSrc = R"(

			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec3 a_Normal;
			layout(location = 2) in vec2 a_Texture;
		
			uniform mat4 u_ProjectionViewMatrix;
			uniform mat4 u_ModelMatrix;

			out vec3 v_Position;
			out vec3 v_Normal;
			out vec2 v_Texture;

			void main(){
				v_Position = a_Position;
				v_Normal = a_Normal;
				v_Texture = a_Texture;
				gl_Position = u_ProjectionViewMatrix * u_ModelMatrix * vec4(v_Position, 1.0);
			}
		)";


std::string triangleFragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;
		
			in vec3 v_Position;
			in vec3 v_Normal;
			in vec2 v_Texture;


			void main(){
				color = vec4(v_Texture, 0.0, 1.0);

			}
		)";
#pragma endregion

class ExampleLayer : public Taller::Layer {
public:
	ExampleLayer() : Layer("Example") {

		/* CREATES A TRIANGLE ENTITY */
		Taller::Entity triangle = coord.CreateEntity();
		coord.AddComponent<Taller::TransformComponent>(triangle, glm::vec3(0.0f), glm::vec3(45.0f), glm::vec3(0.25f));
		coord.AddComponent<Taller::StaticMeshComponent>(triangle, "TriangleShader", "axolotl");
		Taller::StaticMeshComponent& axolotl_SM = coord.GetComponent<Taller::StaticMeshComponent>(triangle);

		m_MeshLibrary.Load(axolotl_SM.meshName, "assets/3dmodels/axolotl.tmesh");
		m_ShaderLibrary.Load(axolotl_SM.shaderName, triangleVertexSrc, triangleFragmentSrc);
		//----------------------------------//




		/* CREATES A SQUARE ENTITY */
		Taller::Entity square = coord.CreateEntity();
		coord.AddComponent<Taller::TransformComponent>(square, glm::vec3(-2.0f, 0.0f, 0.0f), glm::vec3(-55.0f, 10.0f, 45.0f), glm::vec3(0.5f));
		coord.AddComponent<Taller::StaticMeshComponent>(square);
		Taller::StaticMeshComponent& cube_SM = coord.GetComponent<Taller::StaticMeshComponent>(square);

		cube_SM.meshName = "Cube";
		m_MeshLibrary.Load(cube_SM.meshName, "assets/3dmodels/Cube.tmesh");

		cube_SM.shaderName = "SquareShader";
		m_ShaderLibrary.Load(cube_SM.shaderName, "assets/shaders/squareColor.glsl");
		//----------------------------------//



		/* CREATES THE CAMERA */
		Taller::Entity camera = coord.CreateEntity();
		coord.AddComponent<Taller::CameraComponent>(camera, 45.0f,1.7f, 0.1f, 100.0f);
		coord.AddComponent<Taller::TransformComponent>(camera);
		coord.GroupEntity(camera, "cam");

		cam = coord.GetComponent<Taller::CameraComponent>(camera);
		transformCam = coord.GetComponent<Taller::TransformComponent>(camera);
		transformCam.location = glm::vec3(0.0f, 0.0f, 5.0f);
		transformCam.rotation = glm::vec3(0.0f, 0.0f, 0.0f);

		cam.viewProjection = Taller::ComponentOperations::CalculateCameraViewProjection(cam, transformCam);
		//----------------------------------//


		coord.AddQueryRequirement<Taller::TransformComponent>();
		coord.AddQueryRequirement<Taller::StaticMeshComponent>();
		renderSignature = coord.RegisterQueryRequirement();

	};

	void OnUpdate(Taller::Timestep timestep) override {

		TL_PROFILE_FUNCTION();


		{
			TL_PROFILE_SCOPE("Render preparation");
			Taller::RenderCommand::SetClearColor({ 1.0f, 0.0f, 1.0f, 1.0f });
			Taller::RenderCommand::Clear();
		}
		
		for (auto entities : coord.QueryEntitiesBySignature(renderSignature)) {
			Taller::TransformComponent& transform = coord.GetComponent<Taller::TransformComponent>(entities.GetId());
			Taller::StaticMeshComponent& staticMesh = coord.GetComponent<Taller::StaticMeshComponent>(entities.GetId());

			{
				TL_PROFILE_SCOPE("Render");

				Taller::Renderer::BeginScene(cam.viewProjection);

				auto shader = m_ShaderLibrary.Get(staticMesh.shaderName);
				Taller::Renderer::Submit(shader, m_MeshLibrary.Get(staticMesh.meshName), transform.location, transform.rotation, transform.scale);

				Taller::Renderer::EndScene();
			}
		}
	}

	void OnEvent(Taller::Event& e) override {
		TL_LOG_INFO(true, "Layer event : %s", e.ToString());
	}

	virtual void OnImGuiRender() override {
		TL_PROFILE_FUNCTION();

		static bool show = true;
		ImGui::ShowDemoWindow(&show);
	}


private:
	Taller::ShaderLibrary m_ShaderLibrary;
	Taller::MeshLibrary m_MeshLibrary;

	Taller::TransformComponent& transformCam = Taller::TransformComponent();
	Taller::CameraComponent& cam = Taller::CameraComponent();

	Taller::Coordinator& coord = Taller::Application::Get().GetCoordinator();

	Taller::Signature renderSignature;
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