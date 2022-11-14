#pragma once
#include "RendererAPI.h"
#include "glm/glm.hpp"
#include "VertexArray.h"

namespace Hazel {
	class RenderCommand {
	public:
		inline static void Init() { s_RendererAPI->Init(); }
		inline static void SetViewPort(unsigned int width, unsigned int height) { s_RendererAPI->SetViewPort(width, height); }
		inline static void Clear() { s_RendererAPI->Clear(); }
		inline static void SetClearColor(const glm::vec4& color) { s_RendererAPI->SetClearColor(color); }
		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray) { s_RendererAPI->DrawIndexed(vertexArray); }

	private:
		static RendererAPI* s_RendererAPI;
	};
}