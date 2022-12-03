#pragma once
#include "Hazel/Renderer/Framebuffer.h"

namespace Hazel {
	class OpenGLFramebuffer : public Framebuffer {
	public:
		OpenGLFramebuffer(FramebufferSpecification& spec);
		virtual ~OpenGLFramebuffer();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual FramebufferSpecification GetSpecifition() override { return m_Specification; };
		virtual uint32_t GetColorAttachmentRendererID() override { return m_ColorAttachment; };

		void Invalidate();
	private:
		FramebufferSpecification m_Specification;

		uint32_t m_RendererID = 0;
		uint32_t m_ColorAttachment = 0, m_DepthAttachment = 0;

	};
}