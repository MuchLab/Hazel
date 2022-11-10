#include <Hazel.h>
#include "imGui/imgui.h"

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
			uniform mat4 u_Transform;

			void main()
			{
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			out vec4 color;
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

		m_Shader.reset(Hazel::Shader::Create(vertexSrc, fragmentSrc));

		//////////////////////////////////////////////////
		// SQUARE ////////////////////////////////////////
		//////////////////////////////////////////////////

		std::string squareVertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			void main()
			{
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string squareFragmentSrc = R"(
			#version 330 core
			
			out vec4 color;
			uniform vec3 u_Color;

			void main()
			{
				color = vec4(u_Color, 1.0f);
			}
		)";

		float squareVertices[4 * 3] = {
			//Position			
		   -0.5f,   0.5f,  0.0f,	
		   -0.5f,  -0.5f,  0.0f,	
			0.5f,   0.5f,  0.0f,	
			0.5f,  -0.5f,  0.0f,	
		};

		uint32_t squareIndices[6] = { 0, 1, 2, 3, 2, 1 };

		m_SquareVertexArray.reset(Hazel::VertexArray::Create());

		std::shared_ptr<Hazel::VertexBuffer> squareVB;
		squareVB.reset(Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		Hazel::BufferLayout squareLayout = {
			{ "a_Position", Hazel::ShaderDataType::Float3 }
		};

		squareVB->SetLayout(squareLayout);
		m_SquareVertexArray->AddVertexBuffer(squareVB);

		std::shared_ptr<Hazel::IndexBuffer> squareIB;
		squareIB.reset(Hazel::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVertexArray->SetIndexBuffer(squareIB);

		m_SquareShader.reset(Hazel::Shader::Create(squareVertexSrc, squareFragmentSrc));

		
	}

	void OnUpdate(Hazel::Timestep ts) override
	{
		if (Hazel::Input::IsKeyPressed(HZ_KEY_RIGHT))
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_LEFT))
			m_CameraPosition.x += m_CameraMoveSpeed * ts;
		if (Hazel::Input::IsKeyPressed(HZ_KEY_UP))
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_DOWN))
			m_CameraPosition.y += m_CameraMoveSpeed * ts;

		if (Hazel::Input::IsKeyPressed(HZ_KEY_A))
			m_CameraRotation -= m_CameraRotateSpeed * ts;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_D))
			m_CameraRotation += m_CameraRotateSpeed * ts;

		Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Hazel::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Hazel::Renderer::BeginScene(m_Camera);


		
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 1.0f));
		//m_SquareMatrix = glm::translate(m_SquareMatrix, glm::vec3(8.0f, 4.5f, 0.5f));
		for (int y = 0; y < 20; y++)
		{
			
			for (int x = 0; x < 20; x++)
			{
				std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_SquareShader)->UploadUniformFloat3("u_Color", m_SquareColor);
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Hazel::Renderer::Submit(m_SquareShader, m_SquareVertexArray, transform);
			}
		}
		
		
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_SquarePosition);
		Hazel::Renderer::Submit(m_Shader, m_VertexArray, transform);

		Hazel::Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Color");
		ImGui::ColorEdit3("Color", glm::value_ptr(m_SquareColor));
		ImGui::End();

		ImGui::Begin("Position");
		ImGui::SliderFloat("xSlider", &m_SquarePosition.x, -1.6f, 1.6f);
		ImGui::SliderFloat("ySlider", &m_SquarePosition.y, -0.9f, 0.9f);
		ImGui::End();

	}

	void OnEvent(Hazel::Event& event) override
	{
	}

private:
	std::shared_ptr<Hazel::Shader> m_Shader;
	std::shared_ptr<Hazel::VertexArray> m_VertexArray;
	std::shared_ptr<Hazel::Shader> m_SquareShader;
	std::shared_ptr<Hazel::VertexArray> m_SquareVertexArray;

	Hazel::OrthographicCamera m_Camera;

	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 2.0f;

	float m_CameraRotation = 0.0f;
	float m_CameraRotateSpeed = 180.0f;

	glm::vec3 m_SquareColor = glm::vec3(0.8f, 0.2f, 0.5f);
	glm::vec3 m_SquarePosition = glm::vec3(0.0f);
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