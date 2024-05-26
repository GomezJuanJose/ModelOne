#pragma once

#include "Taller/Renderer/RendererAPI.h"

namespace Taller {
	class OpenGLRendererAPI : public RendererAPI{
	public:
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const AssetRef<VertexArray>& vertexArray) override;
	};
}
