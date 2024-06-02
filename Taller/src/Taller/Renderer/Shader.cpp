#include "tlpch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Taller {
	AssetRef<Shader> Shader::Create(const std::string& filePath) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None: TL_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return std::make_shared<OpenGLShader>(filePath);
		}

		TL_ASSERT(false, "Unknown Renderer API!!");
		return nullptr;
	}


	AssetRef<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None: TL_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);
		}

		TL_ASSERT(false, "Unknown Renderer API!!");
		return nullptr;
	}










	void ShaderLibrary::Add(const std::string& name, const AssetRef<Shader>& shader) {
		TL_ASSERT(!Exist(name), "Shader already exists!");
		m_Shaders[name] = shader;
	}

	void ShaderLibrary::Add(const AssetRef<Shader>& shader) {
		auto& name = shader->GetName();
		Add(name, shader);
	}

	AssetRef<Shader> ShaderLibrary::Load(const std::string& filePath) {
		auto shader = Shader::Create(filePath);
		Add(shader);
		return shader;
	}

	AssetRef<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filePath) {
		auto shader = Shader::Create(filePath);
		Add(name, shader);
		return shader;
	}

	AssetRef<Shader> ShaderLibrary::Load(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) {
		auto shader = Shader::Create(name, vertexSrc, fragmentSrc);
		Add(name, shader);
		return shader;
	}

	AssetRef<Shader> ShaderLibrary::Get(const std::string& name) {
		TL_ASSERT(Exist(name), "Shader not found");
		return m_Shaders[name];
	}

	bool ShaderLibrary::Exist(const std::string& name) const {
		return m_Shaders.find(name) != m_Shaders.end();
	}

}