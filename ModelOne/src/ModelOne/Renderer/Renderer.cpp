#include "mopch.h"
#include "Renderer.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include <glad/glad.h>


#include "ModelOne/Renderer/Shader.h"
#include "ModelOne/Renderer/Texture.h"


#include "ModelOne/ECS/Components.h"

namespace ModelOne {

	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	void Renderer::Init() {
		
		MO_PROFILE_FUNCTION();

		RenderCommand::Init();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height) {
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(BeginSceneData& beginSceneData) {
		m_SceneData->ViewProjectionMatrix = beginSceneData.Camera;

		m_SceneData->AmbientColor = beginSceneData.AmbientColor;
		m_SceneData->AmbientIntensity = beginSceneData.AmbientIntensity;

		m_SceneData->DiffuseColor = beginSceneData.DiffuseColor;
		m_SceneData->DiffuseDirection = beginSceneData.DiffuseDirection;
		m_SceneData->DiffuseIntensity = beginSceneData.DiffuseIntensity;
		
		m_SceneData->LightSpaceMatrix = beginSceneData.LightProjectionView;
		m_SceneData->ShadowMapping = beginSceneData.ShadowMapping;
		
		m_SceneData->Gamma = beginSceneData.Gamma;
		

		RenderCommand::BindTextureUnit(0, m_SceneData->ShadowMapping);
	}

	void Renderer::EndScene() {

	}

	void Renderer::Submit(const AssetRef<Shader>& shader, const AssetRef<VertexArray>& vertexArray, const glm::vec3& location, const glm::vec3& rotation, const glm::vec3& scale) {

		shader->Bind();

		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat3("u_AmbientLightColor", m_SceneData->AmbientColor);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat("u_AmbientLightIntensity", m_SceneData->AmbientIntensity);

		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat3("u_DiffuseLightDirection", m_SceneData->DiffuseDirection);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat3("u_DiffuseLightColor", m_SceneData->DiffuseColor);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat("u_DiffuseLightIntensity", m_SceneData->DiffuseIntensity);

		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ProjectionViewMatrix", m_SceneData->ViewProjectionMatrix);
		
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_LightSpaceMatrix", m_SceneData->LightSpaceMatrix);

		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformInt("u_ShadowMap", 0);
		
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat("u_Gamma", m_SceneData->Gamma);

		
		/* TODO This is not efficient and introduce the Gimbal locks, change it to quaternions*/
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), location)
			* glm::rotate(glm::mat4(1.0), glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f))
			* glm::rotate(glm::mat4(1.0), glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f))
			* glm::rotate(glm::mat4(1.0), glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f))
			* glm::scale(glm::mat4(1.0f), scale);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ModelMatrix", transform);//TODO Material system
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat3("u_NormalMatrix", glm::mat3(glm::transpose(glm::inverse(transform))));//TODO Material system

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

	void Renderer::Submit(const AssetRef<Shader>& shader, const AssetRef<Texture>& texture, const AssetRef<VertexArray>& vertexArray, const glm::vec3& location, const glm::vec3& rotation, const glm::vec3& scale) {

		texture->Bind(1);
		
		std::dynamic_pointer_cast<OpenGLShader>(shader)->Bind();

		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat3("u_AmbientLightColor", m_SceneData->AmbientColor);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat("u_AmbientLightIntensity", m_SceneData->AmbientIntensity);

		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat3("u_DiffuseLightDirection", m_SceneData->DiffuseDirection);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat3("u_DiffuseLightColor", m_SceneData->DiffuseColor);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat("u_DiffuseLightIntensity", m_SceneData->DiffuseIntensity);

		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ProjectionViewMatrix", m_SceneData->ViewProjectionMatrix);

		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_LightSpaceMatrix", m_SceneData->LightSpaceMatrix);
		
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformInt("u_ShadowMap", 0);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformInt("u_Texture", 1);
		
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat("u_Gamma", m_SceneData->Gamma);


		/* TODO This is not efficient and introduce the Gimbal locks, change it to quaternions*/
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), location)
			* glm::rotate(glm::mat4(1.0), glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f))
			* glm::rotate(glm::mat4(1.0), glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f))
			* glm::rotate(glm::mat4(1.0), glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f))
			* glm::scale(glm::mat4(1.0f), scale);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ModelMatrix", transform);//TODO Material system
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat3("u_NormalMatrix", glm::mat3(glm::transpose(glm::inverse(transform))));//TODO Material system

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}