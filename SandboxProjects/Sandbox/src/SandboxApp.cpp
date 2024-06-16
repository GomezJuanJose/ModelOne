#include <Taller.h>
#include <Taller/Core/EntryPoint.h>
#include <Taller/Core/Window.h>
#include <Platform/OpenGL/OpenGLShader.h>
#include "ImGui/imgui.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//TODO A veces crashea el buffer al cargar el modelado
//TODO Valorar si compensa hacer una plantilla AssetLibrary<T>
//TODO Revisar sistema de coordenadas (si son cm o m)

#pragma region PLACE_HOLDER_DATA
std::string triangleVertexSrc = R"(

			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec3 a_Normal;
			layout(location = 2) in vec2 a_Texcoord;
		
			uniform mat4 u_ProjectionViewMatrix;
			uniform mat4 u_ModelMatrix;

			out vec3 v_Position;
			out vec3 v_Normal;
			out vec2 v_Texcoord;

			void main(){
				v_Position = a_Position;
				v_Normal = a_Normal;
				v_Texcoord = a_Texcoord;
				gl_Position = u_ProjectionViewMatrix * u_ModelMatrix * vec4(v_Position, 1.0);
			}
		)";


std::string triangleFragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;
		
			in vec3 v_Position;
			in vec3 v_Normal;
			in vec2 v_Texcoord;

			uniform sampler2D u_Texture;


			void main(){
				vec4 c = texture(u_Texture, v_Texcoord);
				if(c.a <= 0.1) // Or whichever comparison here
					discard;
				color = c;
				//color = vec4(v_Texcoord, 0.0, 1.0);
			}
		)";
#pragma endregion

class ExampleLayer : public Taller::Layer {
public:
	ExampleLayer() : Layer("Example") {

		/* CREATES A AXOLOTL ENTITY */
		Taller::Entity triangle = coord.CreateEntity();
		coord.AddComponent<Taller::TransformComponent>(triangle, glm::vec3(0.0f), glm::vec3(-45.0f), glm::vec3(0.25f));
		coord.AddComponent<Taller::StaticMeshComponent>(triangle, "TriangleShader", "axolotl", "cyan_texture");
		Taller::StaticMeshComponent& axolotl_SM = coord.GetComponent<Taller::StaticMeshComponent>(triangle);
		coord.GroupEntity(triangle,"Movable");

		m_Texture2DLibrary.Load(axolotl_SM.textureName, "assets/textures/axolotl_cyan.png");
		m_MeshLibrary.Load(axolotl_SM.meshName, "assets/3dmodels/axolotl.tmesh");
		m_ShaderLibrary.Load(axolotl_SM.shaderName, triangleVertexSrc, triangleFragmentSrc);
		//----------------------------------//


		/* CREATES A GROUND PLANE ENTITY */
		Taller::Entity plane = coord.CreateEntity();
		coord.AddComponent<Taller::TransformComponent>(plane, glm::vec3(0.0f, -0.5f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(15.0f));
		coord.AddComponent<Taller::StaticMeshComponent>(plane, "SquareShader", "", "square");
		Taller::StaticMeshComponent& plane_SM = coord.GetComponent<Taller::StaticMeshComponent>(plane);
		coord.GroupEntity(plane, "Static");

		m_MeshLibrary.Load(plane_SM.meshName, "assets/3dmodels/Square.tmesh");
		m_ShaderLibrary.Load(plane_SM.shaderName, "assets/shaders/squareColor.glsl");
		//----------------------------------//


		/* CREATES A SQUARE ENTITY */
		Taller::Entity square = coord.CreateEntity();
		coord.AddComponent<Taller::TransformComponent>(square, glm::vec3(-2.0f, 0.0f, 0.0f), glm::vec3(-55.0f, 10.0f, 45.0f), glm::vec3(0.25f));
		coord.AddComponent<Taller::StaticMeshComponent>(square);
		Taller::StaticMeshComponent& cube_SM = coord.GetComponent<Taller::StaticMeshComponent>(square);
		coord.GroupEntity(square, "Movable");

		cube_SM.meshName = "Cube";
		m_MeshLibrary.Load(cube_SM.meshName, "assets/3dmodels/Cube.tmesh");

		cube_SM.shaderName = "SquareShader";
		//----------------------------------//



		/* CREATES THE CAMERA */
		Taller::Entity camera = coord.CreateEntity();
		coord.AddComponent<Taller::CameraComponent>(camera, 45.0f, 16.0f/9.0f, 0.1f, 100.0f);
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
			Taller::RenderCommand::SetClearColor({ 0.7f, 0.0f, 0.7f, 1.0f });
			Taller::RenderCommand::Clear();
		}
		
		for (auto entities : coord.GetEntitiesByGroup("cam")) {
				transformCam.location.x = camx;
				transformCam.location.y = camy;
				transformCam.location.z = camz;

				transformCam.rotation.x = glm::degrees(camRotx);
				transformCam.rotation.y = glm::degrees(camRoty);
				transformCam.rotation.z = glm::degrees(camRotz);


				//TODO	This mantains an scale and proportion relative to aspect ration, implement this inside the renderer
				float aspectRation = ((float)(Taller::Application::Get().GetWindow().GetWidth()) / (float)(Taller::Application::Get().GetWindow().GetHeight()));
				cam.projection = glm::perspective(glm::radians(45.0f), aspectRation, 0.1f, 100.0f);
				//--------------------------------------

				//TODO Implement this in the renderer in each frame because the camera will be always transforming
				cam.viewProjection = Taller::ComponentOperations::CalculateCameraViewProjection(cam, transformCam);
		}

		for (auto entities : coord.QueryEntitiesBySignature(renderSignature)) {
			Taller::TransformComponent& transform = coord.GetComponent<Taller::TransformComponent>(entities.GetId());
			Taller::StaticMeshComponent& staticMesh = coord.GetComponent<Taller::StaticMeshComponent>(entities.GetId());

			if (coord.EntityBelongsToGroup(entities, "Movable")) {
				transform.rotation.x = glm::degrees(x);
				transform.rotation.y = glm::degrees(y);
				transform.rotation.z = glm::degrees(z);
			}

			{
				TL_PROFILE_SCOPE("Render");

				Taller::Renderer::BeginScene(cam.viewProjection);

				auto shader = m_ShaderLibrary.Get(staticMesh.shaderName);

				auto texture = staticMesh.textureName != "" ? m_Texture2DLibrary.Get(staticMesh.textureName) : nullptr;
				auto mesh = m_MeshLibrary.Get(staticMesh.meshName);


				if (texture) {
					Taller::Renderer::Submit(shader, texture, mesh, transform.location, transform.rotation, transform.scale);
				}
				else {
					Taller::Renderer::Submit(shader, mesh, transform.location, transform.rotation, transform.scale);

				}

				Taller::Renderer::EndScene();
			}
		}
	}

	void OnEvent(Taller::Event& e) override {
		TL_LOG_INFO(true, "Layer event : %s", e.ToString());
	}

	virtual void OnImGuiRender() override {
		TL_PROFILE_FUNCTION();
		ImGui::Begin("Debug 3D Model!");

		ImGui::SliderAngle("X", &x);
		ImGui::SliderAngle("Y", &y);
		ImGui::SliderAngle("Z", &z);
		ImGui::Separator();

		ImGui::End();

		ImGui::Begin("Camera controls!");

		ImGui::SliderFloat("X", &camx, -100.0f, 100.0f, "X = %f");
		ImGui::SliderFloat("Y", &camy, -100.0f, 100.0f, "Y = %f");
		ImGui::SliderFloat("Z", &camz, -100.0f, 100.0f, "Z = %f");

		ImGui::SliderAngle("ROT X", &camRotx);
		ImGui::SliderAngle("ROT Y", &camRoty);
		ImGui::SliderAngle("ROT Z", &camRotz);

		ImGui::End();
		
		
	}


private:
	Taller::ShaderLibrary m_ShaderLibrary;
	Taller::MeshLibrary m_MeshLibrary;
	Taller::Texture2DLibrary m_Texture2DLibrary;

	Taller::TransformComponent& transformCam = Taller::TransformComponent();
	Taller::CameraComponent& cam = Taller::CameraComponent();

	Taller::Coordinator& coord = Taller::Application::Get().GetCoordinator();

	Taller::Signature renderSignature;

	float x;
	float y;
	float z;

	float camx = 0;
	float camy = 1.0;
	float camz = 5.0;
	float camRotx = 0;
	float camRoty = 0;
	float camRotz = 0;
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