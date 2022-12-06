#pragma once
#include <Hazel.h>

namespace Hazel {
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;

	private:
		OrthographicCameraController m_CameraController;

		ShaderLibrary m_ShaderLibrary;
		Ref<VertexArray> m_SquareVA;
		Ref<Framebuffer> m_Framebuffer;

		glm::vec4 m_SquareColor = glm::vec4(0.8f, 0.2f, 0.5f, 1.0f);

		Ref<Texture2D> m_CheckerboardTexture;
		Ref<Texture2D> m_ChernoLogoTexture;
		float m_Rotation = 0.0f;

		Ref<Scene> m_ActiveScene;
		entt::entity m_SquareEntity;

		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

		bool m_ViewportFocused = false, m_ViewportHovered = false;
	};
}