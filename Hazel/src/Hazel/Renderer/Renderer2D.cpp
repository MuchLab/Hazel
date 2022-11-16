#include "hzpch.h"
#include "Renderer2D.h"

#include "RenderCommand.h"
#include "Hazel/Core/Input.h"
#include "Hazel/Core/KeyCodes.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Hazel {

	Renderer2D::Renderer2DStorage* Renderer2D::s_Data = new Renderer2D::Renderer2DStorage();

	void Renderer2D::Init()
	{
		float vertices[4 * 3] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};
		uint32_t indices[6] = { 0, 1, 2, 0, 3, 2 };

		s_Data->VertexArray = VertexArray::Create();

		auto vb = VertexBuffer::Create(vertices, sizeof(vertices));
		BufferLayout layout = {
			{ "a_Position", ShaderDataType::Float3 }
		};

		vb->SetLayout(layout);
		s_Data->VertexArray->AddVertexBuffer(vb);

		auto ib = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		s_Data->VertexArray->SetIndexBuffer(ib);

		s_Data->Shader = Shader::Create("assets/shaders/Square.glsl");
	}

	void Renderer2D::BeginScene(OrthographicCamera& camera)
	{
		s_Data->Shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->Shader)->UploadUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->Shader)->UploadUniformMat4("u_Transform", glm::mat4(1.0f));

	}
	void Renderer2D::EndScene()
	{

	}
	void Renderer2D::ShutDown()
	{
		delete s_Data;
	}
	void Renderer2D::DrawQuad(glm::vec2 position, glm::vec2 size, glm::vec4 color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}
	void Renderer2D::DrawQuad(glm::vec3 position, glm::vec2 size, glm::vec4 color)
	{
		s_Data->Shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->Shader)->UploadUniformFloat4("u_Color", color);
		s_Data->VertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->VertexArray);
	}
}