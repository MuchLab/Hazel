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
		virtual void AddVertexBuffer(std::shared_ptr<VertexBuffer>& buffer) override;
		virtual void SetIndexBuffer(std::shared_ptr<IndexBuffer>& buffer) override;
		virtual inline const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const { return m_VertexBuffers; };
		virtual inline const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; };

	private:
		uint32_t m_RendererID;
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
	};

}


