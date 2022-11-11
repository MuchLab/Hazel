#pragma once

#include "Hazel/Renderer/Texture.h"

namespace Hazel {

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& path);
		~OpenGLTexture2D();

		inline virtual int GetHeight() const override { return m_Height; };
		inline virtual int GetWidth() const override { return m_Width; };

		virtual void Bind(const uint32_t slot = 0) const override;
	private:
		uint32_t m_RendererID;
		int m_Height, m_Width;
		std::string m_Path;
	};

}
