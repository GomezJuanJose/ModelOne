#include "tlpch.h"
#include "Texture.h"

#include "Taller/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Taller {
	AssetRef<Texture2D> Texture2D::Create(const std::string& filePath) {
		
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None: TL_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return std::make_shared<OpenGLTexture2D>(filePath);
		}

		TL_ASSERT(false, "Unknown Renderer API!!");
		return nullptr;
	}





	void Texture2DLibrary::Add(const std::string& name, const AssetRef<Texture2D>& texture) {
		TL_ASSERT(!Exist(name), "Texture already exists!!");
		m_Textures[name] = texture;
	}
	
	AssetRef<Texture2D> Texture2DLibrary::Load(const std::string& name, const std::string& filePath) {
		auto texture = Texture2D::Create(filePath);
		Add(name, texture);
		return texture;
	}
	
	AssetRef<Texture2D> Texture2DLibrary::Get(const std::string& name) {
		TL_ASSERT(Exist(name), "Texture not found");
		return m_Textures[name];
	}
	
	bool Texture2DLibrary::Exist(const std::string& name) const {
		return m_Textures.find(name) != m_Textures.end();
	}
}