#include "mopch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace ModelOne {
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
		switch (type) {
			case ShaderDataType::Float:		return GL_FLOAT;
			case ShaderDataType::Float2:	return GL_FLOAT;
			case ShaderDataType::Float3:	return GL_FLOAT;
			case ShaderDataType::Float4:	return GL_FLOAT;
			case ShaderDataType::Mat3:		return GL_FLOAT;
			case ShaderDataType::Mat4:		return GL_FLOAT;
			case ShaderDataType::Int:		return GL_INT;
			case ShaderDataType::Int2:		return GL_INT;
			case ShaderDataType::Int3:		return GL_INT;
			case ShaderDataType::Int4:		return GL_INT;
			case ShaderDataType::Bool:		return GL_BOOL;
		}

		MO_ASSERT(false, "Unknown shader type");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray() {
		
		MO_PROFILE_RENDERER_FUNCTION();

		glCreateVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray() {

		MO_PROFILE_RENDERER_FUNCTION();

		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const {

		MO_PROFILE_RENDERER_FUNCTION();

		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const {

		MO_PROFILE_RENDERER_FUNCTION();

		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const AssetRef<VertexBuffer>& vertexBuffer) {

		MO_PROFILE_RENDERER_FUNCTION();

		MO_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex buffer has no layouts!!!");
		
		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();
		
		//Binds all the attributes of the vertex buffer layout
		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout) {
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset
			);

			index++;
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const AssetRef<IndexBuffer>& indexBuffer) {

		MO_PROFILE_RENDERER_FUNCTION();

		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}
}
