#include "hzpch.h"
#include "SubTexture2D.h"

namespace Hazel {
	SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max, const glm::vec2& size)
		: m_Texture(texture), m_Size(size)
	{
		HZ_PROFILE_FUNCTION();

		m_TexCoords[0] = { min.x, min.y };
		m_TexCoords[1] = { max.x, min.y };
		m_TexCoords[2] = { max.x, max.y };
		m_TexCoords[3] = { min.x, max.y };
	}
	Ref<SubTexture2D> SubTexture2D::CreateFromCoords(const Ref<Texture2D>& texture, glm::vec2 sheetIndex, glm::vec2 cellSize, glm::vec2 offset)
	{
		HZ_PROFILE_FUNCTION();

		int width = texture->GetWidth();
		int height = texture->GetHeight();
		glm::vec2 min = { sheetIndex.x * (cellSize.x / width), sheetIndex.y * (cellSize.y / height) };
		glm::vec2 max = { (sheetIndex.x + offset.x) * (cellSize.x / width), (sheetIndex.y + offset.y) * (cellSize.y / height)};
		return CreateRef<SubTexture2D>(texture, min, max, offset);
	}
}