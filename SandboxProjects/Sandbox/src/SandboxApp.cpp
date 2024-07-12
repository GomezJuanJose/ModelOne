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

//TODO Hacer traduccion de grados a una direccion vec3 para la directional light
//TODO Que la proyeccion ortogonal mire en la misma direccion que la directional light

//TODO Mejorar la formula del bias para el shadow acne
//TODO Buscar una mejor forma para resolver el peter panning, no usar front face culling porque se carga los planos y eso en algunos casos no es de interes(como el modelado)
//TODO Sincronizar la iluminacion difusa con el casteo de sombras (que vaya a la misma direccion)

//TODO Hacer que solo haga una vez el calculo de la direccion de la luz

#pragma region PLACE_HOLDER_DATA
std::string triangleVertexSrc = R"(

			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec3 a_Normal;
			layout(location = 2) in vec2 a_Texcoord;
		
			uniform mat4 u_ProjectionViewMatrix;
			uniform mat4 u_ModelMatrix;
			uniform mat3 u_NormalMatrix;

			uniform mat4 u_LightSpaceMatrix; // For shadow map

			out vec3 v_Position;
			out vec3 v_Normal;
			out vec2 v_Texcoord;

			out vec3 v_FragmentPosition;

			out vec4 v_FragPosLightSpace; // For shadow map

			void main(){
				v_Position = a_Position;
				v_Normal = u_NormalMatrix * a_Normal;
				v_Texcoord = a_Texcoord;
				
				v_FragmentPosition = vec3(u_ModelMatrix * vec4(v_Position, 1.0));

				v_FragPosLightSpace = u_LightSpaceMatrix * vec4(v_FragmentPosition, 1.0);

				gl_Position = u_ProjectionViewMatrix * u_ModelMatrix * vec4(v_Position, 1.0);
			}
		)";


std::string triangleFragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 finalColor;
		
			in vec3 v_Position;
			in vec3 v_Normal;
			in vec2 v_Texcoord;

			in vec3 v_FragmentPosition;

			in vec4 v_FragPosLightSpace;

			uniform sampler2D u_Texture;
			uniform sampler2D u_ShadowMap;

			uniform vec3 u_AmbientLightColor;
			uniform float u_AmbientLightIntensity;

			uniform vec3 u_DiffuseLightDirection;
			uniform vec3 u_DiffuseLightColor;
			uniform float u_DiffuseLightIntensity;
			
			uniform float u_Gamma;


			float ShadowCalculation(vec4 fragPosLightSpace){
				//Perform perspective divide
				vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
				
				//Transforms NDC to range [0,1]
				projCoords = projCoords * 0.5 + 0.5;


				//Avoids oversampling by fustrum side
				if (projCoords.z > 1.0){
					return 0.0;
				}


				float closestDepth = texture(u_ShadowMap, projCoords.xy).r;
				float currentDepth = projCoords.z;
				
				//Prevents shadow acne
				vec3 normal = v_Normal;
				vec3 lightDir = normalize(u_DiffuseLightDirection);
				float bias = max(0.05 * (1.0 - dot(normal,lightDir)), 0.005);

				float shadow = 0.0;
				vec2 texelSize = 1.0 / textureSize(u_ShadowMap, 0);
				for(int x = -1; x <= 1; ++x){
					for(int y = -1; y <= 1; ++y){
						float pcfDepth = texture(u_ShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
						shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
					}
				}
				shadow /= 9.0;

				//float shadow = currentDepth - bias > closestDepth ? 1.0f : 0.0f;

				

				return shadow;
			}


			void main(){
				vec4 c = texture(u_Texture, v_Texcoord);
				if(c.a <= 0.1) // Or whichever comparison here
					discard;

				//Ambient light
				vec3 ambientLight = vec3(1.0) * vec3(u_AmbientLightColor) * u_AmbientLightIntensity;
			
				//Directional light
				vec3 normal = normalize(v_Normal);

				/*this is more for a point light like ilumination*/
				//vec3 lightDirection = normalize(u_DiffuseLightDirection - v_FragmentPosition);
				//float diffuse = max(dot(normal, lightDirection), 0.0);

				vec3 lightDirection = normalize(u_DiffuseLightDirection);
				float diffuse = max(dot(normal, lightDirection), 0.0);
				vec3 diffuseLight = diffuse * u_DiffuseLightColor * u_DiffuseLightIntensity;
			
				//Shadows
				float shadow = ShadowCalculation(v_FragPosLightSpace);

				vec4 fragmentColor = c * vec4(ambientLight + (1.0 - shadow) * (diffuseLight), 1.0);
				finalColor = vec4(pow(fragmentColor.rgb, vec3(1/u_Gamma)), 1.0);
				//color = vec4(v_Texcoord, 0.0, 1.0);
			}
		)";
#pragma endregion

class ExampleLayer : public Taller::Layer {
public:
	ExampleLayer() : Layer("Example") {

		//Creates ambien light
		Taller::Entity light = coord.CreateEntity();
		coord.AddComponent<Taller::AmbientLightComponent>(light, glm::vec3(1.0f), 0.8f);
		coord.AddComponent<Taller::DirectionalLightComponent>(light, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f), 0.3f);
		coord.GroupEntity(light, "generalLight");



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


		/* CREATES A CUBE ENTITY */
		Taller::Entity cube = coord.CreateEntity();
		coord.AddComponent<Taller::TransformComponent>(cube, glm::vec3(-2.0f, 0.0f, 0.0f), glm::vec3(-55.0f, 10.0f, 45.0f), glm::vec3(0.25f));
		coord.AddComponent<Taller::StaticMeshComponent>(cube, "CubeShader", "", "Cube");
		Taller::StaticMeshComponent& cube_SM = coord.GetComponent<Taller::StaticMeshComponent>(cube);
		coord.GroupEntity(cube, "Movable");

		m_MeshLibrary.Load(cube_SM.meshName, "assets/3dmodels/Cube.tmesh");
		m_ShaderLibrary.Load(cube_SM.shaderName, "assets/shaders/cube.glsl");
		//----------------------------------//

		/* CREATES A GROUND PLANE ENTITY */
		Taller::Entity plane = coord.CreateEntity();
		coord.AddComponent<Taller::TransformComponent>(plane, glm::vec3(0.0f, -0.5f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(15.0f));
		coord.AddComponent<Taller::StaticMeshComponent>(plane, "CubeShader", "", "square");
		Taller::StaticMeshComponent& plane_SM = coord.GetComponent<Taller::StaticMeshComponent>(plane);
		coord.GroupEntity(plane, "Static");

		m_MeshLibrary.Load(plane_SM.meshName, "assets/3dmodels/Square.tmesh");
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

		Taller::FrameBufferSpecification frameBufferSpecification;
		frameBufferSpecification.Height = 4096;
		frameBufferSpecification.Width = 4096;
		frameBufferSpecification.Attachments = { Taller::FrameBufferTextureFormat::DEPTH_COMPONENT };
		m_ShadowMap = Taller::FrameBuffer::Create(frameBufferSpecification);

		m_ShaderLibrary.Load("simpleDepth", "assets/shaders/simpleDepth.glsl");
	};

	void OnUpdate(Taller::Timestep timestep) override {

		TL_PROFILE_FUNCTION();

		/*SHADOW PASS*/
		
		{
			TL_PROFILE_SCOPE("Shadow pass");
			
			Taller::RenderCommand::Clear();
			Taller::RenderCommand::SetViewport(0, 0, m_ShadowMap->GetSpecification().Width, m_ShadowMap->GetSpecification().Height);

			float near_plane = 0.1f;
			float far_plane = 10.0f;
			glm::mat4 lightProjection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, near_plane, far_plane);
			glm::mat4 lightView = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f),
				glm::vec3(0.0f, 0.0f, 0.0f),
				glm::vec3(1.0f, 1.0f, 1.0f));

			m_ShadowMap->Bind();

			for (auto entities : coord.QueryEntitiesBySignature(renderSignature)) {
				Taller::TransformComponent& transform = coord.GetComponent<Taller::TransformComponent>(entities.GetId());
				Taller::StaticMeshComponent& staticMesh = coord.GetComponent<Taller::StaticMeshComponent>(entities.GetId());

				{
					Taller::Renderer::BeginSceneData beginSceneData_ShadowMapping;
					beginSceneData_ShadowMapping.LightProjectionView = lightProjection * lightView;
					Taller::Renderer::BeginScene(beginSceneData_ShadowMapping);

					auto shader = m_ShaderLibrary.Get("simpleDepth");
					auto mesh = m_MeshLibrary.Get(staticMesh.meshName);


					Taller::Renderer::Submit(shader, mesh, transform.location, transform.rotation, transform.scale);


					Taller::Renderer::EndScene();
				}
			}

			m_ShadowMap->Unbind();
		}



		/*RENDER PASS*/
		{
			TL_PROFILE_SCOPE("Render pass");
			
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

			{
				TL_PROFILE_SCOPE("Render preparation");

				Taller::RenderCommand::SetViewport(0, 0, Taller::Application::Get().GetWindow().GetWidth(), Taller::Application::Get().GetWindow().GetHeight());
				Taller::RenderCommand::SetClearColor({ 0.7f, 0.0f, 0.7f, 1.0f });
				Taller::RenderCommand::Clear();
			}


			Taller::AmbientLightComponent ambientLightComp;// It makes a copy of it
			Taller::DirectionalLightComponent directionalLightComp;// It makes a copy of it
			for (auto ambientLight : coord.GetEntitiesByGroup("generalLight")) {
				Taller::AmbientLightComponent& alComp = coord.GetComponent<Taller::AmbientLightComponent>(ambientLight);
				Taller::DirectionalLightComponent& dlComp = coord.GetComponent<Taller::DirectionalLightComponent>(ambientLight);
				ambientLightComp = Taller::AmbientLightComponent(alComp.color, alComp.intensity);
				directionalLightComp = Taller::DirectionalLightComponent(dlComp.direction, dlComp.color, dlComp.intensity);
				break;
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
					TL_PROFILE_SCOPE("Render scene");

					Taller::Renderer::BeginSceneData beginSceneData_Render;
					beginSceneData_Render.Camera = cam.viewProjection;
					beginSceneData_Render.AmbientColor = ambientLightComp.color;
					beginSceneData_Render.AmbientIntensity = ambientLightComp.intensity;
					beginSceneData_Render.DiffuseDirection = directionalLightComp.direction;
					beginSceneData_Render.DiffuseColor = directionalLightComp.color;
					beginSceneData_Render.DiffuseIntensity = directionalLightComp.intensity;
					float near_plane = 0.1f;
					float far_plane = 10.0f;
					beginSceneData_Render.ShadowMapping = m_ShadowMap->GetDepthAttachmentRendererID();
					glm::mat4 lightProjection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, near_plane, far_plane);
					glm::mat4 lightView = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f),
						glm::vec3(0.0f, 0.0f, 0.0f),
						glm::vec3(1.0f, 1.0f, 1.0f));
					beginSceneData_Render.LightProjectionView = lightProjection * lightView;
					Taller::Renderer::BeginScene(beginSceneData_Render);

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
		

		ImGui::Begin("Depth buffer");

		ImGui::Image((void*)m_ShadowMap->GetDepthAttachmentRendererID(), ImVec2{ 248,248 });

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

	Taller::AssetRef<Taller::FrameBuffer> m_ShadowMap;

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