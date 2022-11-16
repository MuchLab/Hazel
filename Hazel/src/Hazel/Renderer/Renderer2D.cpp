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
		float vertices[4 * 5] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};
		uint32_t indices[6] = { 0, 1, 2, 0, 3, 2 };

		s_Data->VertexArray = VertexArray::Create();

		auto vb = VertexBuffer::Create(vertices, sizeof(vertices));
		BufferLayout layout = {
			{ "a_Position", ShaderDataType::Float3 },
			{ "a_TexCoord", ShaderDataType::Float2 }
		};

		vb->SetLayout(layout);
		s_Data->VertexArray->AddVertexBuffer(vb);

		auto ib = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		s_Data->VertexArray->SetIndexBuffer(ib);

		s_Data->FlatColorShader = Shader::Create("assets/shaders/Square.glsl");
		s_Data->TextureShader = Shader::Create("assets/shaders/Texture.glsl");

		s_Data->TextureShader->SetInt("u_Texture", 0);
	}

	void Renderer2D::BeginScene(OrthographicCamera& camera)
	{
		s_Data->FlatColorShader->Bind();
		s_Data->FlatColorShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
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
		s_Data->FlatColorShader->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->FlatColorShader->SetMat4("u_Transform", transform);

		s_Data->FlatColorShader->SetFloat4("u_Color", color);
		s_Data->VertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->VertexArray);
	}

	void Renderer2D::DrawQuad(glm::vec2 position, glm::vec2 size, Ref<Texture2D> texture)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture);
	}

	void Renderer2D::DrawQuad(glm::vec3 position, glm::vec2 size, Ref<Texture2D> texture)
	{
		s_Data->TextureShader->Bind();
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->TextureShader->SetMat4("u_Transform", transform);

		s_Data->VertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->VertexArray);
	}
}