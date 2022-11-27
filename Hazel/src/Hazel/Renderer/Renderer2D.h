#pragma once

#include "glm/glm.hpp"
#include "VertexArray.h"
#include "Shader.h"
#include "OrthographicCamera.h"
#include "Texture.h"

namespace Hazel {
	class Renderer2D
	{
	public:
		static void Init();
		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();
		static void ShutDown();
		static void Flush();

		static void DrawQuad(glm::vec3 position, glm::vec2 size, glm::vec4 color, float tilingFactor = 1.0f);
		static void DrawQuad(glm::vec2 position, glm::vec2 size, glm::vec4 color, float tilingFactor = 1.0f);
		static void DrawQuad(glm::vec2 position, glm::vec2 size, Ref<Texture2D> texture, float tilingFactor = 1.0f, glm::vec4 tintColor = glm::vec4(1.0f));
		static void DrawQuad(glm::vec3 position, glm::vec2 size, Ref<Texture2D> texture, float tilingFactor = 1.0f, glm::vec4 tintColor = glm::vec4(1.0f));

		static void DrawRotatedQuad(glm::vec2 position, glm::vec2 size, float rotation, glm::vec4 color, float tilingFactor = 1.0f);
		static void DrawRotatedQuad(glm::vec3 position, glm::vec2 size, float rotation, glm::vec4 color, float tilingFactor = 1.0f);
		static void DrawRotatedQuad(glm::vec2 position, glm::vec2 size, float rotation, Ref<Texture2D> texture, float tilingFactor = 1.0f, glm::vec4 tintColor = glm::vec4(1.0f));
		static void DrawRotatedQuad(glm::vec3 position, glm::vec2 size, float rotation, Ref<Texture2D> texture, float tilingFactor = 1.0f, glm::vec4 tintColor = glm::vec4(1.0f));
	private:
		struct QuadVertex
		{
			glm::vec3 Position;
			glm::vec4 Color;
			glm::vec2 TexCoord;
			float TexIndex;
			float TilingFactor;
		};

		struct Renderer2DData
		{
			const uint32_t MaxQuads = 10000;
			const uint32_t MaxVertices = MaxQuads * 4;
			const uint32_t MaxIndices = MaxQuads * 6;
			static const uint32_t MaxTextureSlots = 32;

			QuadVertex* QuadVertexBufferBase = nullptr;
			QuadVertex* QuadVertexBufferPtr = nullptr;

			Ref<VertexBuffer> QuadVertexBuffer;
			Ref<VertexArray> QuadVertexArray;
			Ref<Shader> TextureShader;
			Ref<Texture2D> WhiteTexture;
			std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;

			uint32_t TextureSlotIndex = 1;
			uint32_t QuadIndexCount = 0;
		};
		static Renderer2DData s_Data;
	};
}