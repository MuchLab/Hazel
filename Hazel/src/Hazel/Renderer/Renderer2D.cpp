#include "hzpch.h"
#include "Renderer2D.h"

#include "RenderCommand.h"
#include "Hazel/Core/Input.h"
#include "Hazel/Core/KeyCodes.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Hazel {

	Renderer2D::Renderer2DData Renderer2D::s_Data = Renderer2D::Renderer2DData();

	void Renderer2D::Init()
	{
		HZ_PROFILE_FUNCTION();

		s_Data.QuadVertexArray = VertexArray::Create();

		s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
		BufferLayout layout = {
			{ "a_Position", ShaderDataType::Float4 },
			{ "a_Color", ShaderDataType::Float4 },
			{ "a_TexCoord", ShaderDataType::Float2 },
			{ "a_TexIndex", ShaderDataType::Float },
			{ "a_TilingFactor", ShaderDataType::Float }
		};

		s_Data.QuadVertexBuffer->SetLayout(layout);
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);
		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		auto ib = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
		s_Data.QuadVertexArray->SetIndexBuffer(ib);
		delete[] quadIndices;

		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t data = 0xffffffff;
		s_Data.WhiteTexture->SetData(&data, sizeof(uint32_t));

		int32_t samplers[s_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
			samplers[i] = i;

		s_Data.TextureShader = Shader::Create("assets/shaders/Renderer2D.glsl");
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetIntArray("u_Textures", s_Data.MaxTextureSlots, samplers);

		s_Data.TextureSlots[0] = s_Data.WhiteTexture;

		s_Data.QuadPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		s_Data.QuadPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}

	void Renderer2D::BeginScene(OrthographicCamera& camera)
	{
		HZ_PROFILE_FUNCTION();

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		HZ_PROFILE_FUNCTION();

		auto& viewProjectionMatrix = camera.GetProjection() * glm::inverse(transform);

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", viewProjectionMatrix);

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::EndScene()
	{
		HZ_PROFILE_FUNCTION();

		//Set Data
		uint32_t dataSize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer2D::ShutDown()
	{
		HZ_PROFILE_FUNCTION();
	}

	void Renderer2D::Flush()
	{
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
			s_Data.TextureSlots[i]->Bind(i);

		RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
		s_Data.Stat.DrawCalls++;
	}

	void Renderer2D::DrawQuad(glm::vec2 position, glm::vec2 size, glm::vec4 color, float tilingFactor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color, tilingFactor);
	}

	void Renderer2D::FlushAndReset()
	{
		EndScene();

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::DrawQuad(glm::vec3 position, glm::vec2 size, glm::vec4 color, float tilingFactor)
	{
		HZ_PROFILE_FUNCTION();

		const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f } );

		DrawQuad(transform, color, tilingFactor);

#ifdef OLD_PATH
		s_Data.TextureShader->SetFloat("u_TilingFactor", 1.0f);
		s_Data.WhiteTexture->Bind();
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data.TextureShader->SetMat4("u_Transform", transform);
		s_Data.QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
#endif
	}

	void Renderer2D::DrawQuad(glm::vec2 position, glm::vec2 size, Ref<Texture2D> texture, float tilingFactor, glm::vec4 tintColor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(glm::vec3 position, glm::vec2 size, Ref<Texture2D> texture, float tilingFactor, glm::vec4 tintColor)
	{
		HZ_PROFILE_FUNCTION();

		const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });


		DrawQuad(transform, texture, tilingFactor, tintColor);

#ifdef OLD_PATH
		s_Data.TextureShader->Bind();
		
		s_Data.TextureShader->SetFloat4("u_Color", tintColor);
		s_Data.TextureShader->SetFloat("u_TilingFactor", tilingFactor);
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data.TextureShader->SetMat4("u_Transform", transform);

		s_Data.QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
#endif
	}

	void Renderer2D::DrawQuad(glm::vec2 position, Ref<SubTexture2D> subTexture, float tilingFactor, glm::vec4 tintColor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, subTexture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(glm::vec3 position, Ref<SubTexture2D> subTexture, float tilingFactor, glm::vec4 tintColor)
	{
		HZ_PROFILE_FUNCTION();

		if (s_Data.QuadIndexCount >= s_Data.MaxIndices)
			FlushAndReset();

		auto texture = subTexture->GetTexture();
		const glm::vec2* texCoords = subTexture->GetTexCoords();
		const glm::vec2 size = subTexture->GetSize();

		float texIndex = 0.0f;

		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i].get() == *texture.get())
			{
				texIndex = (float)i;
				break;
			}
		}

		if (texIndex == 0.0f)
		{
			texIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadPositions[0];
		s_Data.QuadVertexBufferPtr->Color = tintColor;
		s_Data.QuadVertexBufferPtr->TexCoord = texCoords[0];
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;

		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadPositions[1];
		s_Data.QuadVertexBufferPtr->Color = tintColor;
		s_Data.QuadVertexBufferPtr->TexCoord = texCoords[1];
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;

		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadPositions[2];
		s_Data.QuadVertexBufferPtr->Color = tintColor;
		s_Data.QuadVertexBufferPtr->TexCoord = texCoords[2];
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;

		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadPositions[3];
		s_Data.QuadVertexBufferPtr->Color = tintColor;
		s_Data.QuadVertexBufferPtr->TexCoord = texCoords[3];
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;

		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;
		s_Data.Stat.QuadCount++;
	}

	void Renderer2D::DrawQuad(glm::mat4 transform, glm::vec4 color, float tilingFactor)
	{
		HZ_PROFILE_FUNCTION();

		if (s_Data.QuadIndexCount >= s_Data.MaxIndices)
			FlushAndReset();

		const float texIndex = 0.0f;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadPositions[0];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;

		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadPositions[1];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;

		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadPositions[2];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;

		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadPositions[3];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;

		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;
		s_Data.Stat.QuadCount++;
	}

	void Renderer2D::DrawQuad(glm::mat4 transform, Ref<Texture2D> texture, float tilingFactor, glm::vec4 tintColor)
	{
		if (s_Data.QuadIndexCount >= s_Data.MaxIndices)
			FlushAndReset();

		float texIndex = 0.0f;

		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i].get() == *texture.get())
			{
				texIndex = (float)i;
				break;
			}
		}

		if (texIndex == 0.0f)
		{
			texIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadPositions[0];
		s_Data.QuadVertexBufferPtr->Color = tintColor;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;

		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadPositions[1];
		s_Data.QuadVertexBufferPtr->Color = tintColor;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;

		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadPositions[2];
		s_Data.QuadVertexBufferPtr->Color = tintColor;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;

		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadPositions[3];
		s_Data.QuadVertexBufferPtr->Color = tintColor;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;

		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;
		s_Data.Stat.QuadCount++;
	}


	void Renderer2D::DrawRotatedQuad(glm::vec2 position, glm::vec2 size, float rotation, glm::vec4 color, float tilingFactor)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color, tilingFactor);
	}
	void Renderer2D::DrawRotatedQuad(glm::vec3 position, glm::vec2 size, float rotation, glm::vec4 color, float tilingFactor)
	{
		HZ_PROFILE_FUNCTION();

		if (s_Data.QuadIndexCount >= s_Data.MaxIndices)
			FlushAndReset();

		const float texIndex = 0.0f;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f }) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadPositions[0];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadPositions[1];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadPositions[2];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadPositions[3];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;
		s_Data.Stat.QuadCount++;

#ifdef OLD_PATH
		HZ_PROFILE_FUNCTION();

		s_Data.TextureShader->Bind();

		s_Data.TextureShader->SetFloat4("u_Color", color);
		s_Data.TextureShader->SetFloat("u_TilingFactor", tilingFactor);
		s_Data.WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * 
			glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f }) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data.TextureShader->SetMat4("u_Transform", transform);

		s_Data.QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
#endif
	}
	void Renderer2D::DrawRotatedQuad(glm::vec2 position, glm::vec2 size, float rotation, Ref<Texture2D> texture, float tilingFactor, glm::vec4 tintColor)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
	}
	void Renderer2D::DrawRotatedQuad(glm::vec3 position, glm::vec2 size, float rotation, Ref<Texture2D> texture, float tilingFactor, glm::vec4 tintColor)
	{
		HZ_PROFILE_FUNCTION();

		if (s_Data.QuadIndexCount >= s_Data.MaxIndices)
			FlushAndReset();

		float texIndex = 0.0f;

		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i].get() == *texture.get())
			{
				texIndex = (float)i;
				break;
			}
		}

		if (texIndex == 0.0f)
		{
			texIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f }) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadPositions[0];
		s_Data.QuadVertexBufferPtr->Color = tintColor;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;

		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadPositions[1];
		s_Data.QuadVertexBufferPtr->Color = tintColor;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;

		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadPositions[2];
		s_Data.QuadVertexBufferPtr->Color = tintColor;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;

		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadPositions[3];
		s_Data.QuadVertexBufferPtr->Color = tintColor;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;

		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;
		s_Data.Stat.QuadCount++;

#ifdef OLD_PATH
		s_Data.TextureShader->Bind();

		s_Data.TextureShader->SetFloat4("u_Color", tintColor);
		s_Data.TextureShader->SetFloat("u_TilingFactor", tilingFactor);
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f }) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data.TextureShader->SetMat4("u_Transform", transform);

		s_Data.QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
#endif
	}

	void Renderer2D::DrawRotatedQuad(glm::vec2 position, float rotation, Ref<SubTexture2D> subTexture, float tilingFactor, glm::vec4 tintColor)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, rotation, subTexture, tilingFactor, tintColor);
	}
	void Renderer2D::DrawRotatedQuad(glm::vec3 position, float rotation, Ref<SubTexture2D> subTexture, float tilingFactor, glm::vec4 tintColor)
	{
		HZ_PROFILE_FUNCTION();

		if (s_Data.QuadIndexCount >= s_Data.MaxIndices)
			FlushAndReset();

		auto texture = subTexture->GetTexture();
		const glm::vec2* texCoords = subTexture->GetTexCoords();
		const glm::vec2 size = subTexture->GetSize();

		float texIndex = 0.0f;

		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i].get() == *texture.get())
			{
				texIndex = (float)i;
				break;
			}
		}

		if (texIndex == 0.0f)
		{
			texIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f }) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadPositions[0];
		s_Data.QuadVertexBufferPtr->Color = tintColor;
		s_Data.QuadVertexBufferPtr->TexCoord = texCoords[0];
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;

		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadPositions[1];
		s_Data.QuadVertexBufferPtr->Color = tintColor;
		s_Data.QuadVertexBufferPtr->TexCoord = texCoords[1];
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;

		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadPositions[2];
		s_Data.QuadVertexBufferPtr->Color = tintColor;
		s_Data.QuadVertexBufferPtr->TexCoord = texCoords[2];
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;

		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadPositions[3];
		s_Data.QuadVertexBufferPtr->Color = tintColor;
		s_Data.QuadVertexBufferPtr->TexCoord = texCoords[3];
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;

		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;
		s_Data.Stat.QuadCount++;
	}
}