#include <Hazel.h>
#include "glm/glm.hpp"
#include "imGui/imgui.h"

class ExampleLayer : public Hazel::Layer {
public:
	ExampleLayer()
		: Layer("Example"), m_CameraController(1280.0f / 720.0f, true)
	{

		//////////////////////////////////////////////////
		// SQUARE ////////////////////////////////////////
		//////////////////////////////////////////////////

		float squareVertices[4 * 5] = {
			//Position			
		   -0.5f,   0.5f,  0.0f,  0.0f, 1.0f,
		   -0.5f,  -0.5f,  0.0f,  0.0f, 0.0f,
			0.5f,   0.5f,  0.0f,  1.0f, 1.0f,
			0.5f,  -0.5f,  0.0f,  1.0f, 0.0f
		};

		uint32_t squareIndices[6] = { 0, 1, 2, 3, 2, 1 };

		m_SquareVertexArray.reset(Hazel::VertexArray::Create());

		Hazel::Ref<Hazel::VertexBuffer> squareVB;
		squareVB.reset(Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		Hazel::BufferLayout squareLayout = {
			{ "a_Position", Hazel::ShaderDataType::Float3 },
			{ "a_TexCoord", Hazel::ShaderDataType::Float2 }
		};

		squareVB->SetLayout(squareLayout);
		m_SquareVertexArray->AddVertexBuffer(squareVB);

		Hazel::Ref<Hazel::IndexBuffer> squareIB;
		squareIB.reset(Hazel::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVertexArray->SetIndexBuffer(squareIB);

		m_ShaderLib.Add(Hazel::Shader::Create("assets/shaders/Square.glsl"));

		//////////////////////////////////////////////////
		// ANOTHER SQUARE ////////////////////////////////
		//////////////////////////////////////////////////

		auto& textureShader = m_ShaderLib.Load("assets/shaders/Texture.glsl");

		m_Texture = Hazel::Texture2D::Create("assets/textures/Checkerboard.png");

		m_ChernoLogoTexture = Hazel::Texture2D::Create("assets/textures/ChernoLogo.png");

		std::dynamic_pointer_cast<Hazel::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Hazel::Timestep ts) override
	{
		m_CameraController.OnUpdate(ts);

		Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Hazel::RenderCommand::Clear();

		Hazel::Renderer::BeginScene(m_CameraController.GetCamera());

		
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 1.0f));

		auto& squareShader = m_ShaderLib.Get("Square");
		for (int y = 0; y < 20; y++)
		{
			
			for (int x = 0; x < 20; x++)
			{
				std::dynamic_pointer_cast<Hazel::OpenGLShader>(squareShader)->UploadUniformFloat3("u_Color", m_SquareColor);
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Hazel::Renderer::Submit(squareShader, m_SquareVertexArray, transform);
			}
		}
		auto& textureShader = m_ShaderLib.Get("Texture");
		m_Texture->Bind();
		Hazel::Renderer::Submit(textureShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		m_ChernoLogoTexture->Bind();
		Hazel::Renderer::Submit(textureShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		Hazel::Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Color");
		ImGui::ColorEdit3("Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Hazel::Event& e) override
	{
		m_CameraController.OnEvent(e);
	}

private:
	Hazel::ShaderLibrary m_ShaderLib;

	Hazel::Ref<Hazel::VertexArray> m_SquareVertexArray;

	Hazel::Ref<Hazel::Texture2D>m_Texture;

	Hazel::Ref<Hazel::Texture2D>m_ChernoLogoTexture;

	Hazel::OrthographicCameraController m_CameraController;

	

	glm::vec3 m_SquareColor = glm::vec3(0.8f, 0.2f, 0.5f);
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