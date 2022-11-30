#pragma once
#include "Texture.h"
#include "glm/glm.hpp"

namespace Hazel {
	class SubTexture2D
	{
	public:
		SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2 min, const glm::vec2 max);

		const Ref<Texture2D> GetTexture() const { return m_Texture; }
		const glm::vec2* GetTexCoords() const { return m_TexCoords; }

		static Ref<SubTexture2D> CreateFromCoords(const Ref<Texture2D>& texture, glm::vec2 sheetIndex, glm::vec2 cellSize, glm::vec2 offset = { 1.0f, 1.0f });
	private:
		Ref<Texture2D> m_Texture;
		glm::vec2 m_TexCoords[4];
	};
}