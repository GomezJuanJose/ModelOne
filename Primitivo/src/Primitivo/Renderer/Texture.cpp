#include "plpch.h"
#include "Texture.h"

#include "Primitivo/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Primitivo {
	AssetRef<Texture2D> Texture2D::Create(const std::string& filePath) {
		
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None: TL_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return std::make_shared<OpenGLTexture2D>(filePath);
		}

		TL_ASSERT(false, "Unknown Renderer API!!");
		return nullptr;
	}





	
}