#pragma once

#include "Primitivo/Core/Core.h"



namespace Primitivo {
	class Shader;

	class ShaderLibrary {
	public:
		void Add(const std::string& name, const AssetRef<Shader>& shader);
		void Add(const AssetRef<Shader>& shader);
		AssetRef<Shader> Load(const std::string& filePath);
		AssetRef<Shader> Load(const std::string& name, const std::string& filePath);
		AssetRef<Shader> Load(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);

		AssetRef<Shader> Get(const std::string& name);

		bool Exist(const std::string& name) const;

	private:
		std::unordered_map<std::string, AssetRef<Shader>> m_Shaders;
	};
}
