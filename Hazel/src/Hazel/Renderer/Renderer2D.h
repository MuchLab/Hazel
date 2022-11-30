#pragma once

#include "glm/glm.hpp"
#include "VertexArray.h"
#include "Shader.h"
#include "OrthographicCamera.h"
#include "Texture.h"
#include "SubTexture2D.h"

namespace Hazel {
	class Renderer2D
	{
	private:
		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;
			uint32_t GetTotalVertexCount() { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() { return QuadCount * 6; }
		};

		struct QuadVertex
		{
			glm::vec4 Position;
			glm::vec4 Color;
			glm::vec2 TexCoord;
			float TexIndex;
			float TilingFactor;
		};

		struct Renderer2DData
		{
			const uint32_t MaxQuads = 399;
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

			glm::vec4 QuadPositions[4];
			uint32_t TextureSlotIndex = 1;
			uint32_t QuadIndexCount = 0;

			Statistics Stat;
		};
		static Renderer2DData s_Data;
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
		static void DrawQuad(glm::vec2 position, glm::vec2 size, Ref<SubTexture2D> subTexture, float tilingFactor = 1.0f, glm::vec4 tintColor = glm::vec4(1.0f));
		static void DrawQuad(glm::vec3 position, glm::vec2 size, Ref<SubTexture2D> subTexture, float tilingFactor = 1.0f, glm::vec4 tintColor = glm::vec4(1.0f));


		static void DrawRotatedQuad(glm::vec2 position, glm::vec2 size, float rotation, glm::vec4 color, float tilingFactor = 1.0f);
		static void DrawRotatedQuad(glm::vec3 position, glm::vec2 size, float rotation, glm::vec4 color, float tilingFactor = 1.0f);
		static void DrawRotatedQuad(glm::vec2 position, glm::vec2 size, float rotation, Ref<Texture2D> texture, float tilingFactor = 1.0f, glm::vec4 tintColor = glm::vec4(1.0f));
		static void DrawRotatedQuad(glm::vec3 position, glm::vec2 size, float rotation, Ref<Texture2D> texture, float tilingFactor = 1.0f, glm::vec4 tintColor = glm::vec4(1.0f));
		static void DrawRotatedQuad(glm::vec2 position, glm::vec2 size, float rotation, Ref<SubTexture2D> subTexture, float tilingFactor = 1.0f, glm::vec4 tintColor = glm::vec4(1.0f));
		static void DrawRotatedQuad(glm::vec3 position, glm::vec2 size, float rotation, Ref<SubTexture2D> subTexture, float tilingFactor = 1.0f, glm::vec4 tintColor = glm::vec4(1.0f));


		inline static Statistics GetStat() { return s_Data.Stat; }
		inline static void ResetStat() { memset(&s_Data.Stat, 0, sizeof(Statistics)); }
	
	private:
		static void FlushAndReset();
	};
}