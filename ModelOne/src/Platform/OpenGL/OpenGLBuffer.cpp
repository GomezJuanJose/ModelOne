#include "mopch.h"
#include "OpenGLBuffer.h"

#include <glad/glad.h>



namespace ModelOne {
	/*-------------------------- VERTEX BUFFER --------------------------*/

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size) {

		MO_PROFILE_RENDERER_FUNCTION();

		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer() {

		MO_PROFILE_RENDERER_FUNCTION();

		glDeleteBuffers(1, &m_RendererID);
	}
	
	void OpenGLVertexBuffer::Bind() const {

		MO_PROFILE_RENDERER_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}
	
	void OpenGLVertexBuffer::Unbind() const {

		MO_PROFILE_RENDERER_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}





	/*-------------------------- INDEX BUFFER --------------------------*/

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count) : m_Count(count) {

		MO_PROFILE_RENDERER_FUNCTION();

		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer() {

		MO_PROFILE_RENDERER_FUNCTION();

		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLIndexBuffer::Bind() const {

		MO_PROFILE_RENDERER_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLIndexBuffer::Unbind() const {

		MO_PROFILE_RENDERER_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}