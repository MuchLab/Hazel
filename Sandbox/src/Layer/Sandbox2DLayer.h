#pragma once
#include <Hazel.h>

class Sandbox2DLayer : public Hazel::Layer
{
public:
	Sandbox2DLayer();
	virtual ~Sandbox2DLayer() = default;

	virtual void OnUpdate(Hazel::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Hazel::Event& event) override;

private:
	Hazel::OrthographicCameraController m_CameraController;

	Hazel::ShaderLibrary m_ShaderLibrary;
	Hazel::Ref<Hazel::VertexArray> m_SquareVA;

	glm::vec3 m_SquareColor = glm::vec3(0.8f, 0.2f, 0.5f);
};