#pragma once
#include "Hazel/Renderer/Framebuffer.h"

namespace Hazel {
	class OpenGLFramebuffer : public Framebuffer {
	public:
		OpenGLFramebuffer(FramebufferSpecification& spec);
		virtual ~OpenGLFramebuffer();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual FramebufferSpecification GetSpecifition() override { return m_Specification; };
		virtual uint32_t GetColorAttachmentRendererID() override { return m_ColorAttachment; };
	private:
		FramebufferSpecification m_Specification;

		uint32_t m_RendererID;
		uint32_t m_ColorAttachment, m_DepthAttachment;

	};
}