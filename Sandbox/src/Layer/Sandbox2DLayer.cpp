#include "Sandbox2DLayer.h"

#include "glm/glm.hpp"
#include "imGui/imgui.h"

Sandbox2DLayer::Sandbox2DLayer()
	: m_CameraController(1280.0f / 720.0f, true)
{

	float vertices[4 * 3] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};
	uint32_t indices[6] = { 0, 1, 2, 0, 3, 2 };

	m_SquareVA = Hazel::VertexArray::Create();

	auto vb = Hazel::VertexBuffer::Create(vertices, sizeof(vertices));
	Hazel::BufferLayout layout = {
		{ "a_Position", Hazel::ShaderDataType::Float3 }
	};

	vb->SetLayout(layout);
	m_SquareVA->AddVertexBuffer(vb);

	auto ib = Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
	m_SquareVA->SetIndexBuffer(ib);

	m_ShaderLibrary.Load("assets/shaders/Square.glsl");
}

void Sandbox2DLayer::OnUpdate(Hazel::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Hazel::RenderCommand::Clear();

	Hazel::Renderer::BeginScene(m_CameraController.GetCamera());

	auto& shader = std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_ShaderLibrary.Get("Square"));

	shader->UploadUniformFloat3("u_Color", m_SquareColor);
	Hazel::Renderer::Submit(shader, m_SquareVA);

	Hazel::Renderer::EndScene();
}

void Sandbox2DLayer::OnImGuiRender()
{
	ImGui::Begin("ColorBoard");
	ImGui::ColorEdit3("Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2DLayer::OnEvent(Hazel::Event& event)
{
	m_CameraController.OnEvent(event);
}

