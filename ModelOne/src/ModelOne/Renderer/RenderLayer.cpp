#include "mopch.h"
#include "RenderLayer.h"

#include "ModelOne/Core/Core.h"

#include "ModelOne/Renderer/FrameBuffer.h"
#include "ModelOne/Renderer/RenderCommand.h"
#include "ModelOne/Renderer/Renderer.h"

#include "ModelOne/Renderer/ShaderLibrary.h"
#include "ModelOne/Renderer/Texture2DLibrary.h"
#include "ModelOne/Renderer/MeshLibrary.h"

#include "ModelOne/Renderer/Shader.h"
#include "ModelOne/Renderer/Texture.h"

#include "ModelOne/ECS/Components.h"


#include "ModelOne/FunctionsLibraries/ComponentOperations.h"

namespace ModelOne {
	RenderLayer::RenderLayer() : Layer("Render"), m_CameraComponent(CameraComponent()) {

		m_Coordinator.AddQueryRequirement<AmbientLightComponent>();
		m_Coordinator.AddQueryRequirement<DirectionalLightComponent>();
		m_LightSignature = m_Coordinator.RegisterQueryRequirement();

		m_Coordinator.AddQueryRequirement<TransformComponent>();
		m_Coordinator.AddQueryRequirement<StaticMeshComponent>();
		m_MeshSignature = m_Coordinator.RegisterQueryRequirement();

		m_Coordinator.AddQueryRequirement<CameraComponent>();
		m_Coordinator.AddQueryRequirement<TransformComponent>();
		m_CameraSignature = m_Coordinator.RegisterQueryRequirement();

		FrameBufferSpecification frameBufferSpecification;
		frameBufferSpecification.Height = 4096;
		frameBufferSpecification.Width = 4096;
		frameBufferSpecification.Attachments = { FrameBufferTextureFormat::DEPTH_COMPONENT };
		m_ShadowMap = FrameBuffer::Create(frameBufferSpecification);

		m_ShaderLibrary.Load("depth", "../../assets/shaders/depth.glsl");
		m_ShaderLibrary.Load("object", "../../assets/shaders/object.glsl");
	};

	void RenderLayer::OnUpdate(Timestep timestep) {

		MO_PROFILE_FUNCTION();

		/*SHADOW PASS*/

		{
			MO_PROFILE_SCOPE("Shadow pass");

			RenderCommand::Clear();
			RenderCommand::SetViewport(0, 0, m_ShadowMap->GetSpecification().Width, m_ShadowMap->GetSpecification().Height);

			float near_plane = 0.1f;
			float far_plane = 10.0f;
			glm::mat4 lightProjection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, near_plane, far_plane);
			glm::mat4 lightView = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f),
				glm::vec3(0.0f, 0.0f, 0.0f),
				glm::vec3(1.0f, 1.0f, 1.0f));

			m_ShadowMap->Bind();

			for (auto entities : m_Coordinator.QueryEntitiesBySignature(m_MeshSignature)) {
				TransformComponent& transform = m_Coordinator.GetComponent<TransformComponent>(entities.GetId());
				StaticMeshComponent& staticMesh = m_Coordinator.GetComponent<StaticMeshComponent>(entities.GetId());
				CameraComponent& cam = m_Coordinator.GetComponent<CameraComponent>(entities.GetId());

				{
					Renderer::BeginSceneData beginSceneData_ShadowMapping;
					beginSceneData_ShadowMapping.LightProjectionView = lightProjection * lightView;
					Renderer::BeginScene(beginSceneData_ShadowMapping);

					auto shader = m_ShaderLibrary.Get("depth");
					auto mesh = m_MeshLibrary.Get(staticMesh.meshName);


					Renderer::Submit(shader, mesh, transform.location, transform.rotation, transform.scale);


					Renderer::EndScene();
				}
			}

			m_ShadowMap->Unbind();
		}



		/*RENDER PASS*/
		{
			MO_PROFILE_SCOPE("Render pass");

			for (auto camera : m_Coordinator.QueryEntitiesBySignature(m_CameraSignature)) {
				m_CameraComponent = m_Coordinator.GetComponent<CameraComponent>(camera);
				TransformComponent& transformCam = m_Coordinator.GetComponent<ModelOne::TransformComponent>(camera);
				
				float aspectRation = ((float)(Application::Get().GetWindow().GetWidth()) / (float)(Application::Get().GetWindow().GetHeight()));

				if (!std::isnan(aspectRation)) {//Avoids crash when is minimize the window
					m_CameraComponent.projection = glm::perspective(glm::radians(45.0f), aspectRation, 0.1f, 100.0f);
				}

				m_CameraComponent.viewProjection = ComponentOperations::CalculateCameraViewProjection(m_CameraComponent, transformCam);
			}

			{
				MO_PROFILE_SCOPE("Render preparation");

				RenderCommand::SetViewport(0, 0, Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight());
				RenderCommand::SetClearColor({ 0.7f, 0.0f, 0.7f, 1.0f });
				RenderCommand::Clear();
			}


			AmbientLightComponent ambientLightComp;// It makes a copy of it
			DirectionalLightComponent directionalLightComp;// It makes a copy of it
			for (auto ambientLight : m_Coordinator.QueryEntitiesBySignature(m_LightSignature)) {
				AmbientLightComponent& alComp = m_Coordinator.GetComponent<AmbientLightComponent>(ambientLight);
				DirectionalLightComponent& dlComp = m_Coordinator.GetComponent<DirectionalLightComponent>(ambientLight);
				ambientLightComp = AmbientLightComponent(alComp.color, alComp.intensity);
				directionalLightComp = DirectionalLightComponent(dlComp.direction, dlComp.color, dlComp.intensity);
				break;
			}

			for (auto entities : m_Coordinator.QueryEntitiesBySignature(m_MeshSignature)) {
				TransformComponent& transform = m_Coordinator.GetComponent<TransformComponent>(entities.GetId());
				StaticMeshComponent& staticMesh = m_Coordinator.GetComponent<StaticMeshComponent>(entities.GetId());

				{
					MO_PROFILE_SCOPE("Render scene");

					Renderer::BeginSceneData beginSceneData_Render;
					beginSceneData_Render.Camera = m_CameraComponent.viewProjection;
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
					Renderer::BeginScene(beginSceneData_Render);

					auto shader = m_ShaderLibrary.Get(staticMesh.shaderName);

					auto texture = staticMesh.textureName != "" ? m_Texture2DLibrary.Get(staticMesh.textureName) : nullptr;
					auto mesh = m_MeshLibrary.Get(staticMesh.meshName);


					if (texture) {
						Renderer::Submit(shader, texture, mesh, transform.location, transform.rotation, transform.scale);
					}
					else {
						Renderer::Submit(shader, mesh, transform.location, transform.rotation, transform.scale);

					}

					Renderer::EndScene();
				}
			}

		}
	}
}