#pragma once
#include <Hazel.h>

class ExampleLayer : public Hazel::Layer {
public:
	ExampleLayer();

	virtual void OnUpdate(Hazel::Timestep ts) override;

	virtual void OnImGuiRender() override;

	virtual void OnEvent(Hazel::Event& e) override;

private:
	Hazel::OrthographicCameraController m_CameraController;

	Hazel::ShaderLibrary m_ShaderLib;
	Hazel::Ref<Hazel::VertexArray> m_SquareVertexArray;

	Hazel::Ref<Hazel::Texture2D>m_Texture;
	Hazel::Ref<Hazel::Texture2D>m_ChernoLogoTexture;

	glm::vec3 m_SquareColor = glm::vec3(0.8f, 0.2f, 0.5f);
};