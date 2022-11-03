#include "hzpch.h"
#include "Application.h"

#include "Hazel/Input.h"
#include <glad/glad.h>
namespace Hazel {

	Application* Application::s_Instance = nullptr;

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

	Application::Application()
	{
		HZ_CORE_ASSERT(!s_Instance, "Application already exist!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(HZ_BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverLayer(m_ImGuiLayer);

		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		float vertices[4 * 7] = {
			-0.5f,  0.0f, 0.0f,     0.8f,  0.2f, 0.5f, 0.0f,
			 0.0f, -0.5f, 0.0f,	    0.5f,  0.8f, 0.5f, 0.0f,
			 0.5f,  0.0f, 0.0f,     0.2f,  0.5f, 0.8f, 0.0f,
			 0.0f,  0.5f, 0.0f,     0.8f,  0.5f, 0.1f, 0.0f,
		};

		

		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		BufferLayout layout = {
				{ "a_Position", ShaderDataType::Float3 },
				{ "a_Color", ShaderDataType::Float4 }
		};

		m_VertexBuffer->SetLayout(layout);

		int index = 0;
		auto& elements = layout.GetElements();
		for (auto& element : elements)
		{
			
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(
				index, 
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type), 
				element.Normalized, 
				layout.GetStride(),
				(const void*)element.Offset);
			index++;
		}
		
		//glEnableVertexAttribArray(1);
		//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (const void*)(3 * sizeof(float)));

		unsigned int indices[6] = { 0, 1, 2, 0, 2, 3 };

		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			out vec4 v_Color;

			void main()
			{
				v_Color = a_Color;
				gl_Position = vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Color);
			}
		)";
		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));
	}

	Application::~Application()
	{
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatech<WindowCloseEvent>(HZ_BIND_EVENT_FN(Application::OnWindowClose));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.m_Handled) { break; }
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}
	void Application::PushOverLayer(Layer* layer)
	{
		m_LayerStack.PushOverLayer(layer);
	}

	void Application::Run()
	{
		while (m_Running)
		{
			glClearColor(0.1f, 0.1f, 0.1f, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			m_Shader->Bind();
			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
}