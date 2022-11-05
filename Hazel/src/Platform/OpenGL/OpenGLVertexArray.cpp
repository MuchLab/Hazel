#include "hzpch.h"
#include "OpenGLVertexArray.h"

#include "glad/glad.h"

namespace Hazel {

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case Hazel::ShaderDataType::Float:    return GL_FLOAT;
		case Hazel::ShaderDataType::Float2:   return GL_FLOAT;
		case Hazel::ShaderDataType::Float3:   return GL_FLOAT;
		case Hazel::ShaderDataType::Float4:   return GL_FLOAT;
		case Hazel::ShaderDataType::Mat3:     return GL_FLOAT;
		case Hazel::ShaderDataType::Mat4:     return GL_FLOAT;
		case Hazel::ShaderDataType::Int:      return GL_INT;
		case Hazel::ShaderDataType::Int2:     return GL_INT;
		case Hazel::ShaderDataType::Int3:     return GL_INT;
		case Hazel::ShaderDataType::Int4:     return GL_INT;
		case Hazel::ShaderDataType::Bool:     return GL_BOOL;
		}

		HZ_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_RendererID);
	}
	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind()
	{
		glBindVertexArray(m_RendererID);
	}
	void OpenGLVertexArray::Unbind()
	{
		glBindVertexArray(0);
	}
	void OpenGLVertexArray::AddVertexBuffer(std::shared_ptr<VertexBuffer>& buffer)
	{
		HZ_CORE_ASSERT(buffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout currently!");
		glBindVertexArray(m_RendererID);
		buffer->Bind();
		uint32_t index = 0;
		const auto& layout = buffer->GetLayout();
		for each (const auto& element in layout)
		{
			glEnableVertexAttribArray(index);
			//GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer
			glVertexAttribPointer(
				index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE :GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset);
			index++;
		}
		m_VertexBuffers.push_back(buffer);
	}
	void OpenGLVertexArray::SetIndexBuffer(std::shared_ptr<IndexBuffer>& buffer)
	{
		m_IndexBuffer = buffer;
	}

	
}