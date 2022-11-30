#include "Sandbox2DLayer.h"

#include "glm/glm.hpp"
#include "imGui/imgui.h"


Sandbox2DLayer::Sandbox2DLayer()
	: m_CameraController(1280.0f / 720.0f, true)
{
	HZ_PROFILE_FUNCTION();

	Hazel::Renderer2D::Init();
	m_CheckerboardTexture = Hazel::Texture2D::Create("assets/textures/Checkerboard.png");
	m_ChernoLogoTexture = Hazel::Texture2D::Create("assets/textures/ChernoLogo.png");
}

void Sandbox2DLayer::OnAttach()
{
	HZ_PROFILE_FUNCTION();

	m_Particle.ColorBegin = m_SquareColor;
	m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
	m_Particle.LifeTime = 10.0f;
	m_Particle.Velocity = { 0.0f, 0.0f };
	m_Particle.VelocityVariation = { 3.0f, 1.0f };
	m_Particle.Position = { 0.0f, 0.0f };
}

void Sandbox2DLayer::OnUpdate(Hazel::Timestep ts)
{
	HZ_PROFILE_FUNCTION();

	Hazel::Renderer2D::ResetStat();

	m_Rotation += ts * 60.0f;

	m_CameraController.OnUpdate(ts);
	
	{
		HZ_PROFILE_SCOPE("RenderCommand::Prepare");

		Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Hazel::RenderCommand::Clear();
	}

	{
		HZ_PROFILE_SCOPE("Renerer2D::Draw");

		Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, m_CheckerboardTexture, 10.0f);
		Hazel::Renderer2D::DrawQuad({ -1.5f, 0.0f, 0.1f }, { 1.5f, 1.5f }, m_SquareColor, 10.0f);
		Hazel::Renderer2D::DrawQuad({ 0.0f, 1.5f, 0.1f }, { 1.5f, 1.5f }, m_ChernoLogoTexture, 1.0f, m_SquareColor);
		Hazel::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f, 0.1f }, { 1.0f, 1.0f }, glm::radians(m_Rotation), m_CheckerboardTexture, 10.0f);
		Hazel::Renderer2D::DrawRotatedQuad({ 1.6f, -0.45f, 0.1f }, { 1.0f, 1.0f }, glm::radians(m_Rotation), m_SquareColor, 10.0f);
		Hazel::Renderer2D::EndScene();

		Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
		for (float x = -5.0f; x < 5.0f; x += 0.5f)
		{
			for (float y = -5.0f; y < 5.0f; y += 0.5f)
			{
				glm::vec4 color = { (x + 5.0f) / 10.0f, (y + 5.0f) / 10.0f, 0.8f, 0.5f };
				Hazel::Renderer2D::DrawQuad({ x, y, 0.0f }, { 0.5f, 0.5f }, color);
			}
		}
		Hazel::Renderer2D::EndScene();

		m_Particle.ColorBegin = m_SquareColor;
		if (Hazel::Input::IsMouseButtonPressed(HZ_MOUSE_BUTTON_LEFT))
		{
			auto [x, y] = Hazel::Input::GetMousePos();
			auto width = Hazel::Application::Get().GetWindow().GetWidth();
			auto height = Hazel::Application::Get().GetWindow().GetHeight();

			auto bounds = m_CameraController.GetBounds();
			auto pos = m_CameraController.GetCamera().GetPosition();
			x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
			y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
			m_Particle.Position = { x + pos.x, y + pos.y };
			for (int i = 0; i < 5; i++)
				m_ParticleSystem.Emit(m_Particle);
		}

		m_ParticleSystem.OnUpdate(ts);
		m_ParticleSystem.OnRender(m_CameraController.GetCamera());
	}
}

void Sandbox2DLayer::OnImGuiRender()
{
	HZ_PROFILE_FUNCTION();

	auto stat = Hazel::Renderer2D::GetStat();

	ImGui::Begin("ColorBoard");
	ImGui::Text("Renderer2D Statistics:");
	ImGui::Text("Draw Calls: %d", stat.DrawCalls);
	ImGui::Text("Quad Count: %d", stat.QuadCount);
	ImGui::Text("Vertex Count: %d", stat.GetTotalVertexCount());
	ImGui::Text("Index Count: %d", stat.GetTotalIndexCount());
	ImGui::ColorEdit4("Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2DLayer::OnEvent(Hazel::Event& event)
{
	HZ_PROFILE_FUNCTION();

	m_CameraController.OnEvent(event);
}

