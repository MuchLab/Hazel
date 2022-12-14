#pragma once

namespace Hazel {

	struct FramebufferSpecification
	{
		uint32_t Width, Height;
		bool SwapChainTarget = false;
	};

	class Framebuffer {
	public:

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual FramebufferSpecification GetSpecifition() = 0;
		virtual uint32_t GetColorAttachmentRendererID() = 0;

		static Ref<Framebuffer> Create(FramebufferSpecification& spec);
	};
}