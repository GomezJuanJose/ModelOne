#include "mopch.h"
#include "Texture2DLibrary.h"

#include "ModelOne/Renderer/Texture.h"

namespace ModelOne {
	void Texture2DLibrary::Add(const std::string& name, const AssetRef<Texture2D>& texture) {
		MO_ASSERT(!Exist(name), "Texture already exists!!");
		m_Textures[name] = texture;
	}

	AssetRef<Texture2D> Texture2DLibrary::Load(const std::string& name, const std::string& filePath) {
		auto texture = Texture2D::Create(filePath);
		Add(name, texture);
		return texture;
	}

	AssetRef<Texture2D> Texture2DLibrary::Get(const std::string& name) {
		MO_ASSERT(Exist(name), "Texture not found");
		return m_Textures[name];
	}

	bool Texture2DLibrary::Exist(const std::string& name) const {
		return m_Textures.find(name) != m_Textures.end();
	}
}