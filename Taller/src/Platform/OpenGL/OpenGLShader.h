#pragma once

#include "Taller/Renderer/Shader.h"
#include <glm/glm.hpp>

// TODO REMOVE!!! Avoid to include the OpenGLShader on Sandbox and include glad here!!! for that is necessary a uniforms and materials system
typedef unsigned int GLenum;

namespace Taller {
	class OpenGLShader : public Shader {
	public:
		OpenGLShader(const std::string& filePath);
		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		void UploadUniformInt(const std::string& name, int value);
		
		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformFloat2(const std::string & name, const glm::vec2 & value);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& value);
		
		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

	private:
		std::string ReadFile(const std::string& filePath); // TODO Redo with a proper file system with virtual files
		std::unordered_map<GLenum, std::string> Preprocess(const std::string& source);
		void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);

	private:
		uint32_t m_RendererID;
	};
}
