#pragma once

#include "Taller/Renderer/Buffer.h"

#include <memory>

namespace Taller {
	class VertexArray {
	public:
		virtual ~VertexArray() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const AssetRef<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const AssetRef<IndexBuffer>& indexBuffer) = 0;
	
		virtual const std::vector<AssetRef<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const AssetRef<IndexBuffer>& GetIndexBuffer() const = 0;

		static AssetRef<VertexArray> Create();
	};
}