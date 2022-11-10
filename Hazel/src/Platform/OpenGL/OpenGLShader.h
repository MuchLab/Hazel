#pragma once
#include "Hazel/Renderer/Shader.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace Hazel {
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);

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
		uint32_t m_RendererID;
	};
}


