#pragma once

#include "glm/glm.hpp"
#include "VertexArray.h"
#include "Shader.h"
#include "OrthographicCamera.h"

namespace Hazel {
	class Renderer2D
	{
	public:
		static void Init();
		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();
		static void ShutDown();

		static void DrawQuad(glm::vec2 position, glm::vec2 size, glm::vec4 color);
		static void DrawQuad(glm::vec3 position, glm::vec2 size, glm::vec4 color);
	private:
		struct Renderer2DStorage
		{
			Ref<VertexArray> VertexArray;
			Ref<Shader> Shader;
		};
		static Renderer2DStorage* s_Data;
	};
}