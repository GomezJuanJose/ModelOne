#pragma once

#include "ModelOne/Renderer/RendererAPI.h"

namespace ModelOne {
	class OpenGLRendererAPI : public RendererAPI{
	public:

		virtual void Init() override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const AssetRef<VertexArray>& vertexArray) override;
		virtual void BindTextureUnit(const uint32_t unit, const uint32_t texture) override;
	};
}
