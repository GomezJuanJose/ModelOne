#pragma once

#include <string>


namespace Taller {
	class Shader {
	public:
		~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const  std::string& GetName() const = 0;

		static AssetRef<Shader> Create(const std::string& filePath);
		static AssetRef<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
	};


	

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

