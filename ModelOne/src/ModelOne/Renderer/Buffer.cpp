#include "mopch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace ModelOne {

	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size) {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None: MO_ASSERT(false, "Renderer API not selected! s_RendererAPI set as None!") return nullptr;
		case RendererAPI::API::OpenGL: return new OpenGLVertexBuffer(vertices, size);
		}

		MO_ASSERT(false, "Renderer API not found")
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size) {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None: MO_ASSERT(false, "Renderer API not selected! s_RendererAPI set as None!") return nullptr;
		case RendererAPI::API::OpenGL: return new OpenGLIndexBuffer(indices, size);
		}

		MO_ASSERT(false, "Renderer API not found")
		return nullptr;
	}

}