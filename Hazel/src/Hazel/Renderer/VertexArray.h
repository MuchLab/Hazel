#pragma once

#include "Buffer.h"

namespace Hazel {

	class VertexArray
	{
	public:
		~VertexArray() {}

		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual void AddVertexBuffer(Ref<VertexBuffer>& buffer) = 0;
		virtual void SetIndexBuffer(Ref<IndexBuffer>& buffer) = 0;
		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;

		static VertexArray* Create();
	};

}


