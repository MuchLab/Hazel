#pragma once
#include <Hazel.h>

#include "Profiling/Timer.h"
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

	glm::vec4 m_SquareColor = glm::vec4(0.8f, 0.2f, 0.5f, 1.0f);

	Hazel::Ref<Hazel::Texture2D> m_CheckerboardTexture;

private:
	struct ProfileResult
	{
		const char* Name;
		float Time;
	};

	std::vector<ProfileResult> m_ProfileResults;

#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](ProfileResult profileResult){ m_ProfileResults.push_back(profileResult); })
};