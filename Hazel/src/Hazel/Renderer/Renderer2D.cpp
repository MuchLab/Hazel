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
		HZ_PROFILE_FUNCTION();

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

		s_Data->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t data = 0xffffffff;
		s_Data->WhiteTexture->SetData(&data, sizeof(uint32_t));

		s_Data->TextureShader = Shader::Create("assets/shaders/Renderer2D.glsl");
		s_Data->TextureShader->SetInt("u_Texture", 0);
	}

	void Renderer2D::BeginScene(OrthographicCamera& camera)
	{
		HZ_PROFILE_FUNCTION();

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{
		HZ_PROFILE_FUNCTION();
	}

	void Renderer2D::ShutDown()
	{
		HZ_PROFILE_FUNCTION();

		delete s_Data;
	}

	void Renderer2D::DrawQuad(glm::vec2 position, glm::vec2 size, glm::vec4 color, float tilingFactor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color, tilingFactor);
	}

	void Renderer2D::DrawQuad(glm::vec3 position, glm::vec2 size, glm::vec4 color, float tilingFactor)
	{
		HZ_PROFILE_FUNCTION();

		s_Data->TextureShader->Bind();

		s_Data->TextureShader->SetFloat4("u_Color", color);
		s_Data->TextureShader->SetFloat("u_TilingFactor", tilingFactor);
		s_Data->WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->TextureShader->SetMat4("u_Transform", transform);

		s_Data->VertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->VertexArray);
	}

	void Renderer2D::DrawQuad(glm::vec2 position, glm::vec2 size, Ref<Texture2D> texture, float tilingFactor, glm::vec4 tintColor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(glm::vec3 position, glm::vec2 size, Ref<Texture2D> texture, float tilingFactor, glm::vec4 tintColor)
	{
		HZ_PROFILE_FUNCTION();

		s_Data->TextureShader->Bind();
		
		s_Data->TextureShader->SetFloat4("u_Color", tintColor);
		s_Data->TextureShader->SetFloat("u_TilingFactor", tilingFactor);
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->TextureShader->SetMat4("u_Transform", transform);

		s_Data->VertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->VertexArray);
	}
	void Renderer2D::DrawRotatedQuad(glm::vec2 position, glm::vec2 size, float rotation, glm::vec4 color, float tilingFactor)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color, tilingFactor);
	}
	void Renderer2D::DrawRotatedQuad(glm::vec3 position, glm::vec2 size, float rotation, glm::vec4 color, float tilingFactor)
	{
		HZ_PROFILE_FUNCTION();

		s_Data->TextureShader->Bind();

		s_Data->TextureShader->SetFloat4("u_Color", color);
		s_Data->TextureShader->SetFloat("u_TilingFactor", tilingFactor);
		s_Data->WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * 
			glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f }) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->TextureShader->SetMat4("u_Transform", transform);

		s_Data->VertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->VertexArray);
	}
	void Renderer2D::DrawRotatedQuad(glm::vec2 position, glm::vec2 size, float rotation, Ref<Texture2D> texture, float tilingFactor, glm::vec4 tintColor)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
	}
	void Renderer2D::DrawRotatedQuad(glm::vec3 position, glm::vec2 size, float rotation, Ref<Texture2D> texture, float tilingFactor, glm::vec4 tintColor)
	{
		HZ_PROFILE_FUNCTION();

		s_Data->TextureShader->Bind();

		s_Data->TextureShader->SetFloat4("u_Color", tintColor);
		s_Data->TextureShader->SetFloat("u_TilingFactor", tilingFactor);
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f }) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->TextureShader->SetMat4("u_Transform", transform);

		s_Data->VertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->VertexArray);
	}

}