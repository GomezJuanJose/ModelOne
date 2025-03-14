#pragma once

#include "ModelOne/Renderer/Shader.h"
#include <glm/glm.hpp>

// TODO Remove, avoid to include the OpenGLShader on Sandbox and include glad here for that is necessary a uniforms and materials system
typedef unsigned int GLenum;

namespace ModelOne {
	class OpenGLShader : public Shader {
	public:
		OpenGLShader(const std::string& filePath);
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual const  std::string& GetName() const override { return m_Name; };

		void UploadUniformInt(const std::string& name, int value);
		
		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformFloat2(const std::string & name, const glm::vec2 & value);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& value);
		
		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

	private:
		std::unordered_map<GLenum, std::string> Preprocess(const std::string& source);
		void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);

	private:
		uint32_t m_RendererID;
		std::string m_Name;
	};
}
