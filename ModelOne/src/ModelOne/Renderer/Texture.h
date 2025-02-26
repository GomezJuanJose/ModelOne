#pragma once

#include "ModelOne/Core/Core.h"

namespace ModelOne {
	
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

}
