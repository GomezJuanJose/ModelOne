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

		static void BeginScene(glm::mat4& camera);
		static void EndScene();

		static void Submit(const AssetRef<Shader>& shader, const AssetRef<VertexArray>& vertexArray, const glm::vec3& location = glm::vec3(0.0f), const glm::vec3& rotation = glm::vec3(0.0f), const glm::vec3& scale = glm::vec3(1.0f));
		static void Submit(const AssetRef<Shader>& shader, const AssetRef<Texture>& texture, const AssetRef<VertexArray>& vertexArray, const glm::vec3& location = glm::vec3(0.0f), const glm::vec3& rotation = glm::vec3(0.0f), const glm::vec3& scale = glm::vec3(1.0f));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData {
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* m_SceneData;
	};
}

