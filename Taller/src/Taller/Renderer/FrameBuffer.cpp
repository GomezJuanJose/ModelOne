#include "tlpch.h"
#include "FrameBuffer.h"

#include "Taller/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLFrameBuffer.h"

namespace Taller {
	AssetRef<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& specification) {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None: TL_ASSERT(false, "Renderer API not selected! s_RendererAPI set as None!") return nullptr;
		case RendererAPI::API::OpenGL: return CreateAssetRef<OpenGLFrameBuffer>(specification);
		}

		TL_ASSERT(false, "Renderer API not found")
			return nullptr;
	}
}