#include "mopch.h"
#include "FrameBuffer.h"

#include "ModelOne/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLFrameBuffer.h"
#include "ModelOne/Core/Core.h"

namespace ModelOne {
	AssetRef<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& specification) {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None: MO_ASSERT(false, "Renderer API not selected! s_RendererAPI set as None!") return nullptr;
		case RendererAPI::API::OpenGL: return CreateAssetRef<OpenGLFrameBuffer>(specification);
		}

		MO_ASSERT(false, "Renderer API not found")
			return nullptr;
	}
}