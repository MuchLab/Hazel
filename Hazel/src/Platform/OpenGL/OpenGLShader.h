#pragma once
#include "Hazel/Renderer/Shader.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"



namespace Hazel {
	typedef unsigned int GLenum;

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);

		OpenGLShader(const std::string& filepath);

		~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		void OpenGLShader::UploadUniformInt(const std::string &name, const int value);

		void OpenGLShader::UploadUniformFloat(const std::string &name, const float value);
		void OpenGLShader::UploadUniformFloat2(const std::string &name, const glm::vec2& values);
		void OpenGLShader::UploadUniformFloat3(const std::string &name, const glm::vec3& values);
		void OpenGLShader::UploadUniformFloat4(const std::string &name, const glm::vec4& values);

		void OpenGLShader::UploadUniformMat3(const std::string &name, const glm::mat3& matrix);
		void OpenGLShader::UploadUniformMat4(const std::string &name, const glm::mat4& matrix);



	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);

	private:
		uint32_t m_RendererID;
	};
}


