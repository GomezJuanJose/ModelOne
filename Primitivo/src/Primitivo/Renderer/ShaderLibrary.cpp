#include "plpch.h"
#include "ShaderLibrary.h"

#include "Primitivo/Renderer/Shader.h"

namespace Primitivo {

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