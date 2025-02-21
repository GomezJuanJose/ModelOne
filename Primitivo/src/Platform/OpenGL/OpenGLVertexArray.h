#pragma once

#include "Primitivo/Renderer/VertexArray.h"

namespace Primitivo {
	class OpenGLVertexArray : public VertexArray{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const AssetRef<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const AssetRef<IndexBuffer>& indexBuffer) override;

		virtual const std::vector<AssetRef<VertexBuffer>>& GetVertexBuffers() const override { return m_VertexBuffers; }
		virtual const AssetRef<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; }

	private:
		uint32_t m_RendererID;
		std::vector<AssetRef<VertexBuffer>> m_VertexBuffers;
		AssetRef<IndexBuffer> m_IndexBuffer;
	};
}
