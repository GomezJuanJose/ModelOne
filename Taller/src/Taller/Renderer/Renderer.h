#pragma once

#include "RenderCommand.h"
#include "Taller/Renderer/Shader.h"
#include "Taller/Renderer/Texture.h"
#include "Taller/ECS/Components.h"

namespace Taller {

	class Renderer {
	public:

		static void Init();
		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(glm::mat4& camera, glm::vec3 ambientColor = glm::vec3(1.0f), float ambientIntensity = 0.5f, glm::vec3 diffuseDirection = glm::vec3(1.0f), glm::vec3 diffuseColor = glm::vec3(1.0f), float diffuseIntensity = 1.0f);
		static void EndScene();

		static void Submit(const AssetRef<Shader>& shader, const AssetRef<VertexArray>& vertexArray, const glm::vec3& location = glm::vec3(0.0f), const glm::vec3& rotation = glm::vec3(0.0f), const glm::vec3& scale = glm::vec3(1.0f));
		static void Submit(const AssetRef<Shader>& shader, const AssetRef<Texture>& texture, const AssetRef<VertexArray>& vertexArray, const glm::vec3& location = glm::vec3(0.0f), const glm::vec3& rotation = glm::vec3(0.0f), const glm::vec3& scale = glm::vec3(1.0f));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData {
			glm::mat4 ViewProjectionMatrix;
			
			glm::vec3 AmbientColor;
			float AmbientIntensity;

			glm::vec3 DiffuseColor;
			glm::vec3 DiffuseDirection;
			float DiffuseIntensity;
		};

		static SceneData* m_SceneData;
	};
}

