#pragma once
#include "Hazel/Renderer/VertexArray.h"

namespace Hazel {

	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();

		virtual void Bind() override;
		virtual void Unbind() override;
		virtual void AddVertexBuffer(Ref<VertexBuffer>& buffer) override;
		virtual void SetIndexBuffer(Ref<IndexBuffer>& buffer) override;
		virtual inline const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const { return m_VertexBuffers; };
		virtual inline const Ref<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; };

	private:
		uint32_t m_RendererID;
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffer;
	};

}


