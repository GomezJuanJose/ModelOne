#pragma once

#include "Primitivo/Renderer/FrameBuffer.h"

namespace Primitivo {
	class OpenGLFrameBuffer : public FrameBuffer {
	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& specification);
		virtual ~OpenGLFrameBuffer();
		
		void Invalidate();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual void ClearColorAttachment(uint32_t index, const void* value) override;

		virtual const uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override { TL_ASSERT(index < m_ColorAttachments.size(), "Index out range"); return m_ColorAttachments[index]; }
		virtual const uint32_t GetDepthAttachmentRendererID() const override {return m_DepthAttachment; }

		virtual const FrameBufferSpecification& GetSpecification() const override { return m_Specification; };

	private:
		uint32_t m_RendererID;
		FrameBufferSpecification m_Specification;

		std::vector<FrameBufferTextureSpecification> m_ColorAttachmentSpecifications;
		FrameBufferTextureSpecification m_DepthAttachmentSpecification;

		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment = 0;
	};
}
