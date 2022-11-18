#include "Sandbox2DLayer.h"

#include "glm/glm.hpp"
#include "imGui/imgui.h"


Sandbox2DLayer::Sandbox2DLayer()
	: m_CameraController(1280.0f / 720.0f, true)
{
	HZ_PROFILE_FUNCTION();

	Hazel::Renderer2D::Init();
	m_CheckerboardTexture = Hazel::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2DLayer::OnUpdate(Hazel::Timestep ts)
{
	HZ_PROFILE_FUNCTION();

	m_CameraController.OnUpdate(ts);
	
	{
		HZ_PROFILE_SCOPE("RenderCommand::Prepare");

		Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Hazel::RenderCommand::Clear();
	}

	{
		HZ_PROFILE_SCOPE("Renerer2D::Draw");

		Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Hazel::Renderer2D::DrawQuad({ -0.5f,  0.5f }, { 1.2f, 0.8f }, m_SquareColor);
		Hazel::Renderer2D::DrawQuad({ 0.75f, -0.4f }, { 0.5f, 0.9f }, m_SquareColor);
		Hazel::Renderer2D::DrawQuad({ 0.0f,  0.0f, -0.1f }, { 10.0f, 10.0f }, m_CheckerboardTexture);
		Hazel::Renderer2D::EndScene();
	}
}

void Sandbox2DLayer::OnImGuiRender()
{
	HZ_PROFILE_FUNCTION();

	ImGui::Begin("ColorBoard");
	ImGui::ColorEdit4("Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2DLayer::OnEvent(Hazel::Event& event)
{
	HZ_PROFILE_FUNCTION();

	m_CameraController.OnEvent(event);
}

