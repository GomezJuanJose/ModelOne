#pragma once


namespace Primitivo {

	enum class FrameBufferTextureFormat {
		None = 0,

		//Color
		RGBA8,
		

		//Depth
		DEPTH_COMPONENT,

		//Defaults
		Depth = DEPTH_COMPONENT
	};

	//This is a texture specification
	struct FrameBufferTextureSpecification {
		FrameBufferTextureSpecification() = default;
		FrameBufferTextureSpecification(FrameBufferTextureFormat format) : TextureFormat(format){}

		FrameBufferTextureFormat TextureFormat = FrameBufferTextureFormat::None;
		//TODO filtering/wrap
	};

	//This is the set of vaious textures specifications
	struct FrameBufferAttachmentSpecification {
		FrameBufferAttachmentSpecification() = default;
		FrameBufferAttachmentSpecification(std::initializer_list<FrameBufferTextureSpecification> attachments) : TextureSpecifications(attachments){}

		std::vector<FrameBufferTextureSpecification> TextureSpecifications;
	};


	struct FrameBufferSpecification {
		uint32_t Width;
		uint32_t Height;
		uint32_t Samples = 1;

		FrameBufferAttachmentSpecification Attachments;

		bool SwapChainTarget = false; // If wants to render to screen
	};

	class FrameBuffer {
	public:
		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void ClearColorAttachment(uint32_t index, const void* value) = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual const uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const = 0;
		virtual const uint32_t GetDepthAttachmentRendererID() const = 0;
		virtual const FrameBufferSpecification& GetSpecification() const = 0;

		static AssetRef<FrameBuffer> Create(const FrameBufferSpecification& specification);
	};
}
