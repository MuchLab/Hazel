#include "hzpch.h"
#include "Application.h"

#include "Hazel/Input.h"
#include "Hazel/Renderer/Renderer.h"
namespace Hazel {

	Application* Application::s_Instance = nullptr;

	

	Application::Application()
		: m_Camera(-1.6f, 1.6f, -1.6f, 1.6f)
	{
		HZ_CORE_ASSERT(!s_Instance, "Application already exist!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(HZ_BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverLayer(m_ImGuiLayer);

		//////////////////////////////////////////////////
		// TRANGLE ///////////////////////////////////////
		//////////////////////////////////////////////////

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			out vec4 v_Color;
			uniform mat4 u_ViewProjection;

			void main()
			{
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
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

		float vertices[9 * 7] = {
			//Position				//Color
			-0.75f, -0.5f, 0.0f,     0.8f,  0.2f, 0.5f, 0.0f,		// 0
			-0.25f, -0.5f, 0.0f,	 0.5f,  0.8f, 0.5f, 0.0f,		// 1
			-0.5f,   0.5f, 0.0f,     0.2f,  0.5f, 0.8f, 0.0f,		// 2
		};

		unsigned int indices[3] = { 0, 1, 2 };

		m_VertexArray.reset(VertexArray::Create());

		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		BufferLayout layout = {
				{ "a_Position", ShaderDataType::Float3 },
				{ "a_Color", ShaderDataType::Float4 }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);
		

		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);
		
		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));

		//////////////////////////////////////////////////
		// SQUARE ////////////////////////////////////////
		//////////////////////////////////////////////////

		float squareVertices[4 * 7] = {
			//Position				//Color
			0.25f,  0.5f, 0.0f,		0.8f, 0.2f, 0.5f, 0.0f,		// 0		0		2
			0.25f, -0.5f, 0.0f,		0.2f, 0.5f, 0.8f, 0.0f,		// 1
			0.75f,  0.5f, 0.0f,		0.5f, 0.8f, 0.2f, 0.0f,		// 2
			0.75f, -0.5f, 0.0f,		0.8f, 0.2f, 0.5f, 0.0f		// 3		1		3
		};

		uint32_t squareIndices[6] = { 0, 1, 2, 3, 2, 1 };

		m_SquareVertexArray.reset(VertexArray::Create());
		
		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		squareVB->SetLayout(layout);
		m_SquareVertexArray->AddVertexBuffer(squareVB);

		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVertexArray->SetIndexBuffer(squareIB);

		m_SquareShader.reset(new Shader(vertexSrc, fragmentSrc));
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

			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
			RenderCommand::Clear();
			{
				Renderer::BeginScene(m_Camera);

				Renderer::Submit(m_Shader, m_VertexArray);

				Renderer::EndScene();
			}
			
			{
				Renderer::BeginScene(m_Camera);

				Renderer::Submit(m_SquareShader, m_SquareVertexArray);

				Renderer::EndScene();
			}
			
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