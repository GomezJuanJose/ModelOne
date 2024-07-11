#include "tlpch.h"
#include "OpenGLFrameBuffer.h"

#include <glad/glad.h>

namespace Taller {

	static const uint32_t s_MaxFrameBufferSize = 8192;

	//TODO Move to a more centralize place
	namespace Utils {

		static GLenum TextureTarget(bool isMultisampled) {
			return isMultisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		static void CreateTextures(bool isMultisampled, uint32_t* outID, uint32_t count) {
			glCreateTextures(TextureTarget(isMultisampled), count, outID);
		}

		static void BindTexture(bool isMultisampled, uint32_t id) {
			glBindTexture(TextureTarget(isMultisampled), id);
		}

		static bool IsDepthFormat(FrameBufferTextureFormat format) {
			switch (format) {
			case FrameBufferTextureFormat::DEPTH_COMPONENT: return true;
			}

			return false;
		}

		static void AttachColorTexture(uint32_t id, int samples, GLenum format, uint32_t width, uint32_t height, int index) {
			bool isMultisampled = samples > 1;

			if (isMultisampled) {
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
			}
			else {
				glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(isMultisampled), id, 0);
		}

		static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height) {
			bool isMultisample = samples > 1;
			
			if (isMultisample) {
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
			}
			else {
				//glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);
				glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_FLOAT, nullptr);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
				float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
				glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(isMultisample), id, 0);
		}

		static GLenum TallerFrameBufferTextureFormatToOpenGL(FrameBufferTextureFormat format) {
			switch (format) {
			case FrameBufferTextureFormat::RGBA8: return GL_RGBA8;
			case FrameBufferTextureFormat::DEPTH_COMPONENT: return GL_DEPTH_COMPONENT;
			}

			TL_ASSERT(false, "Format not found!");
			return 0;
		}
	}

	OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& specification) : m_Specification(specification) {
		
		for (auto specification : m_Specification.Attachments.TextureSpecifications) {
			if (!Utils::IsDepthFormat(specification.TextureFormat)) {
				m_ColorAttachmentSpecifications.emplace_back(specification);
			}
			else {
				m_DepthAttachmentSpecification = specification;
			}
		}

		Invalidate();
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer() {
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
		glDeleteTextures(1, &m_DepthAttachment);
	}

	void OpenGLFrameBuffer::Invalidate() {
		if (m_RendererID) {
			glDeleteFramebuffers(1, &m_RendererID);
			glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
			glDeleteTextures(1, &m_DepthAttachment);

			m_ColorAttachments.clear();
			m_DepthAttachment = 0;
		}

		glCreateFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		//Attachmnets

		bool isMultisample = m_Specification.Samples > 1;

		if (m_ColorAttachmentSpecifications.size()) {
			m_ColorAttachments.resize(m_ColorAttachmentSpecifications.size()); //Avoids innecesary allocations and sync both vectors, which allows to check index range once and is valid for both
		
			Utils::CreateTextures(isMultisample, m_ColorAttachments.data(), m_ColorAttachments.size());
		
			for (size_t i = 0; i < m_ColorAttachments.size(); i++) {
				Utils::BindTexture(isMultisample, m_ColorAttachments[i]);
				switch (m_ColorAttachmentSpecifications[i].TextureFormat) {
				case FrameBufferTextureFormat::RGBA8:
					Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_RGBA8, m_Specification.Width, m_Specification.Height, i);
					break;
				}
			}
		}

		if (m_DepthAttachmentSpecification.TextureFormat != FrameBufferTextureFormat::None) {
			Utils::CreateTextures(isMultisample, &m_DepthAttachment, 1);
			Utils::BindTexture(isMultisample, m_DepthAttachment);

			switch (m_DepthAttachmentSpecification.TextureFormat) {
			case FrameBufferTextureFormat::DEPTH_COMPONENT:
				Utils::AttachDepthTexture(m_DepthAttachment, m_Specification.Samples, GL_DEPTH_COMPONENT, GL_DEPTH_ATTACHMENT, m_Specification.Width, m_Specification.Height);
			}
		}

		//Define draw buffers
		if (m_ColorAttachments.size() > 1) {
			TL_ASSERT(m_ColorAttachments.size() <= 4, "At the moment only supports a maximum of 4 color buffers");
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(m_ColorAttachments.size(), buffers);
		}
		else if (m_ColorAttachments.empty()) {
			//Only depth-pass
			glDrawBuffer(GL_NONE);
		}
	}
	
	void OpenGLFrameBuffer::Bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLFrameBuffer::Unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::Resize(uint32_t width, uint32_t height) {
		if (width == 0 || height == 0 || width > s_MaxFrameBufferSize || height > s_MaxFrameBufferSize) {
			TL_LOG_WARNING(true, "Attempted to resize framebuffer to %i %i", width, height);
			return;
		}

		m_Specification.Width = width;
		m_Specification.Height = height;

		Invalidate();
	}

	void OpenGLFrameBuffer::ClearColorAttachment(uint32_t attachmentIndex, const void* value) {
		TL_ASSERT(attachmentIndex < m_ColorAttachments.size(), "Index out of bounds!");

		auto& specification = m_ColorAttachmentSpecifications[attachmentIndex];

		glClearTexImage(m_ColorAttachments[attachmentIndex], 0,
			Utils::TallerFrameBufferTextureFormatToOpenGL(specification.TextureFormat), GL_INT, &value);
	}
}