#include <Hazel.h>
#include "imGui/imgui.h"
#include "Hazel/Renderer/RenderCommand.h"
#include "Hazel/Renderer/Renderer.h"
#include "Hazel/Renderer/Buffer.h"
#include "Hazel/Renderer/VertexArray.h"
#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/OrthographicCamera.h"

class ExampleLayer : public Hazel::Layer {
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(m_Camera.GetPosition())
	{
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

		float vertices[3 * 7] = {
			//Position				  //Color
			-0.25f, -0.25f, 0.0f,     0.8f,  0.2f, 0.5f, 0.0f,		// 0
			 0.25f, -0.25f, 0.0f,	  0.5f,  0.8f, 0.5f, 0.0f,		// 1
			 0.0f,   0.25f, 0.0f,     0.2f,  0.5f, 0.8f, 0.0f,		// 2
		};

		unsigned int indices[3] = { 0, 1, 2 };

		m_VertexArray.reset(Hazel::VertexArray::Create());

		std::shared_ptr<Hazel::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Hazel::VertexBuffer::Create(vertices, sizeof(vertices)));

		Hazel::BufferLayout layout = {
				{ "a_Position", Hazel::ShaderDataType::Float3 },
				{ "a_Color", Hazel::ShaderDataType::Float4 }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);


		std::shared_ptr<Hazel::IndexBuffer> indexBuffer;
		indexBuffer.reset(Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_Shader.reset(new Hazel::Shader(vertexSrc, fragmentSrc));

		//////////////////////////////////////////////////
		// SQUARE ////////////////////////////////////////
		//////////////////////////////////////////////////

		float squareVertices[4 * 7] = {
			//Position					//Color
		   -0.5f,   0.5f, 0.0f,		0.2f, 0.2f, 0.5f, 0.0f,		// 0		0		2
		   -0.5f,  -0.5f, 0.0f,		0.2f, 0.2f, 0.5f, 0.0f,		// 1
			0.5f,   0.5f, 0.0f,		0.2f, 0.2f, 0.5f, 0.0f,		// 2
			0.5f,  -0.5f, 0.0f,		0.2f, 0.2f, 0.5f, 0.0f		// 3		1		3
		};

		uint32_t squareIndices[6] = { 0, 1, 2, 3, 2, 1 };

		m_SquareVertexArray.reset(Hazel::VertexArray::Create());

		std::shared_ptr<Hazel::VertexBuffer> squareVB;
		squareVB.reset(Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		squareVB->SetLayout(layout);
		m_SquareVertexArray->AddVertexBuffer(squareVB);

		std::shared_ptr<Hazel::IndexBuffer> squareIB;
		squareIB.reset(Hazel::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVertexArray->SetIndexBuffer(squareIB);

		m_SquareShader.reset(new Hazel::Shader(vertexSrc, fragmentSrc));
	}

	void OnUpdate() override
	{
		Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Hazel::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Hazel::Renderer::BeginScene(m_Camera);

		Hazel::Renderer::Submit(m_SquareShader, m_SquareVertexArray);

		Hazel::Renderer::Submit(m_Shader, m_VertexArray);

		Hazel::Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
	}

	void OnEvent(Hazel::Event& event) override
	{
		Hazel::EventDispatcher dispatcher(event);
		dispatcher.Dispatech<Hazel::KeyPressedEvent>(HZ_BIND_EVENT_FN(ExampleLayer::OnKeyPressed));
	}

	bool OnKeyPressed(Hazel::KeyPressedEvent& event)
	{
		if (event.GetKeyCode() == HZ_KEY_RIGHT)
			m_CameraPosition.x -= m_CameraMoveSpeed;
		else if (event.GetKeyCode() == HZ_KEY_LEFT)
			m_CameraPosition.x += m_CameraMoveSpeed;
		if (event.GetKeyCode() == HZ_KEY_UP)
			m_CameraPosition.y -= m_CameraMoveSpeed;
		else if (event.GetKeyCode() == HZ_KEY_DOWN)
			m_CameraPosition.y += m_CameraMoveSpeed;
		
		if (event.GetKeyCode() == HZ_KEY_A)
			m_CameraRotation -= m_CameraRotateSpeed;
		else if (event.GetKeyCode() == HZ_KEY_D)
			m_CameraRotation += m_CameraRotateSpeed;

		return false;
	}
private:
	std::shared_ptr<Hazel::Shader> m_Shader;
	std::shared_ptr<Hazel::VertexArray> m_VertexArray;
	std::shared_ptr<Hazel::Shader> m_SquareShader;
	std::shared_ptr<Hazel::VertexArray> m_SquareVertexArray;

	Hazel::OrthographicCamera m_Camera;

	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 0.1f;

	float m_CameraRotation = 0.0f;
	float m_CameraRotateSpeed = 1.0f;
};



class Sandbox : public Hazel::Application
{
public:
	Sandbox();
	~Sandbox();

private:

};

Sandbox::Sandbox()
{
	PushLayer(new ExampleLayer());
}

Sandbox::~Sandbox()
{
}

Hazel::Application* Hazel::CreateApplication()
{
	return new Sandbox();
}