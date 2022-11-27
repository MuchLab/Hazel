#pragma once

#include "glm/glm.hpp"

namespace Hazel {
	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual const std::string GetName() const = 0;
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetInt(const std::string& name, const int value) = 0;
		virtual void SetIntArray(const std::string& name, int count, int* values) = 0;
		virtual void SetFloat(const std::string& name, const float value) = 0;
		virtual void SetFloat3(const std::string& name, const glm::vec3& values) = 0;
		virtual void SetFloat4(const std::string& name, const glm::vec4& values) = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& values) = 0;

		static Ref<Shader> Create(const std::string& filepath);
		static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
	};
	
	class ShaderLibrary
	{
	public:

		void Add(const Ref<Shader> shader);
		void Add(const std::string& name, Ref<Shader> shader);

		Ref<Shader> Load(const std::string& filepath);
		Ref<Shader> Load(const std::string& name, const std::string& filepath);
		Ref<Shader> Get(const std::string& name);

	private:
		bool Exists(const std::string& name) const;
	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};
}
