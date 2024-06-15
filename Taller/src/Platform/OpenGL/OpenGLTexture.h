#pragma once

#include "Taller/Renderer/Texture.h"

namespace Taller {
	class OpenGLTexture2D : public Texture2D{
	public:
		OpenGLTexture2D(const std::string& filePath);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() override { return m_Width; };
		virtual uint32_t GetHeight() override { return m_Height; };

		virtual void Bind(uint32_t slot) override;

	private:
		std::string m_FilePath; // For debug porpouses
		uint32_t m_Width;
		uint32_t m_Height;
		uint32_t m_RendererID;
	};
}
