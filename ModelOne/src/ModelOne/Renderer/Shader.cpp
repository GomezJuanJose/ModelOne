#include "mopch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace ModelOne {
	AssetRef<Shader> Shader::Create(const std::string& filePath) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None: MO_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return std::make_shared<OpenGLShader>(filePath);
		}

		MO_ASSERT(false, "Unknown Renderer API!!");
		return nullptr;
	}


	AssetRef<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None: MO_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);
		}

		MO_ASSERT(false, "Unknown Renderer API!!");
		return nullptr;
	}
}