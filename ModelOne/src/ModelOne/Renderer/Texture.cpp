#include "mopch.h"
#include "Texture.h"

#include "ModelOne/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace ModelOne {
	AssetRef<Texture2D> Texture2D::Create(const std::string& filePath) {
		
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None: MO_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return std::make_shared<OpenGLTexture2D>(filePath);
		}

		MO_ASSERT(false, "Unknown Renderer API!!");
		return nullptr;
	}





	
}