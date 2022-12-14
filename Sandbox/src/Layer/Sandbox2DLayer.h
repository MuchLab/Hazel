#pragma once
#include <Hazel.h>
#include "Particle/ParticleSystem.h"

class Sandbox2DLayer : public Hazel::Layer
{
public:
	Sandbox2DLayer();
	virtual ~Sandbox2DLayer() = default;

	virtual void OnAttach() override;
	virtual void OnUpdate(Hazel::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Hazel::Event& event) override;

private:
	Hazel::OrthographicCameraController m_CameraController;

	Hazel::ShaderLibrary m_ShaderLibrary;
	Hazel::Ref<Hazel::VertexArray> m_SquareVA;
	Hazel::Ref<Hazel::Framebuffer> m_Framebuffer;

	glm::vec4 m_SquareColor = glm::vec4(0.8f, 0.2f, 0.5f, 1.0f);

	Hazel::Ref<Hazel::Texture2D> m_CheckerboardTexture;
	Hazel::Ref<Hazel::Texture2D> m_ChernoLogoTexture;
	float m_Rotation = 0.0f;

	ParticleProps m_Particle;
	ParticleSystem m_ParticleSystem;
};