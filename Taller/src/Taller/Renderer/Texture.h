#pragma once

#include "Taller/Core/Core.h"

namespace Taller {
	
	class Texture {
	public:
		virtual ~Texture(){}

		virtual uint32_t GetWidth() = 0;
		virtual uint32_t GetHeight() = 0;

		virtual void Bind(uint32_t slot = 0) = 0;
	};

	class Texture2D : public Texture {
	public:
		static AssetRef<Texture2D> Create(const std::string& filePath);
	};


	class Texture2DLibrary {
	public:
		void Add(const std::string& name, const AssetRef<Texture2D>& texture);
		AssetRef<Texture2D> Load(const std::string& name, const std::string& filePath);

		AssetRef<Texture2D> Get(const std::string& name);

		bool Exist(const std::string& name) const;

	private:
		std::unordered_map<std::string, AssetRef<Texture2D>> m_Textures;
	};
}
