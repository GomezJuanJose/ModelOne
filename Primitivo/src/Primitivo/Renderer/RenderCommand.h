#pragma once

#include "RendererAPI.h"

namespace Primitivo{
	class RenderCommand {
	public:

		inline static void Init() {
			s_RendererAPI->Init();
		}

		inline static void SetClearColor(const glm::vec4& color) {
			s_RendererAPI->SetClearColor(color);
		}

		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
			s_RendererAPI->SetViewport(x, y, width, height);
		}

		inline static void Clear() {
			s_RendererAPI->Clear();
		}

		inline static void DrawIndexed(const AssetRef<VertexArray>& vertexArray) {
			s_RendererAPI->DrawIndexed(vertexArray);
		}

		inline static void BindTextureUnit(const uint32_t unit, const uint32_t texture) {
			s_RendererAPI->BindTextureUnit(unit, texture);
		}


	private:
		static RendererAPI* s_RendererAPI;
	};
}