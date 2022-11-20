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

		static void DrawQuad(glm::vec3 position, glm::vec2 size, glm::vec4 color, float tilingFactor = 1.0f);
		static void DrawQuad(glm::vec2 position, glm::vec2 size, glm::vec4 color, float tilingFactor = 1.0f);
		static void DrawQuad(glm::vec2 position, glm::vec2 size, Ref<Texture2D> texture, float tilingFactor = 1.0f, glm::vec4 tintColor = glm::vec4(1.0f));
		static void DrawQuad(glm::vec3 position, glm::vec2 size, Ref<Texture2D> texture, float tilingFactor = 1.0f, glm::vec4 tintColor = glm::vec4(1.0f));

		static void DrawRotatedQuad(glm::vec2 position, glm::vec2 size, float rotation, glm::vec4 color, float tilingFactor = 1.0f);
		static void DrawRotatedQuad(glm::vec3 position, glm::vec2 size, float rotation, glm::vec4 color, float tilingFactor = 1.0f);
		static void DrawRotatedQuad(glm::vec2 position, glm::vec2 size, float rotation, Ref<Texture2D> texture, float tilingFactor = 1.0f, glm::vec4 tintColor = glm::vec4(1.0f));
		static void DrawRotatedQuad(glm::vec3 position, glm::vec2 size, float rotation, Ref<Texture2D> texture, float tilingFactor = 1.0f, glm::vec4 tintColor = glm::vec4(1.0f));
	private:
		struct Renderer2DStorage
		{
			Ref<VertexArray> VertexArray;
			Ref<Shader> TextureShader;
			Ref<Texture2D> WhiteTexture;
		};
		static Renderer2DStorage* s_Data;
	};
}