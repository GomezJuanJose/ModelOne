#pragma once

#include <string>


namespace Primitivo {
	class Shader {
	public:
		~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const  std::string& GetName() const = 0;

		static AssetRef<Shader> Create(const std::string& filePath);
		static AssetRef<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
	};
}

