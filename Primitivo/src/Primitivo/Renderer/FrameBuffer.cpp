#include "plpch.h"
#include "FrameBuffer.h"

#include "Primitivo/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLFrameBuffer.h"
#include "Primitivo/Core/Core.h"

namespace Primitivo {
	AssetRef<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& specification) {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None: TL_ASSERT(false, "Renderer API not selected! s_RendererAPI set as None!") return nullptr;
		case RendererAPI::API::OpenGL: return CreateAssetRef<OpenGLFrameBuffer>(specification);
		}

		TL_ASSERT(false, "Renderer API not found")
			return nullptr;
	}
}