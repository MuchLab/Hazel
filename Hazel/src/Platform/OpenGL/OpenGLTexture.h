#pragma once

#include "Hazel/Renderer/Texture.h"
#include <glad/glad.h>

namespace Hazel {

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const uint32_t width, const uint32_t height);
		OpenGLTexture2D(const std::string& path);
		~OpenGLTexture2D();

		inline virtual int GetHeight() const override { return m_Height; };
		inline virtual int GetWidth() const override { return m_Width; };

		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(const uint32_t slot = 0) const override;

		virtual bool operator==(Texture& other)
		{
			return m_RendererID == ((OpenGLTexture2D&)other).m_RendererID;
		}
	private:
		uint32_t m_RendererID;
		int m_Height, m_Width;
		std::string m_Path;
		GLenum m_InternalFormat, m_DataFormat;
	};

}
