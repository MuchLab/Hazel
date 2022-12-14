#include "EditorLayer.h"

#include "glm/glm.hpp"
#include "imGui/imgui.h"

namespace Hazel {
	EditorLayer::EditorLayer()
		: m_CameraController(5.0f, 1280.0f / 720.0f, true)
	{
		HZ_PROFILE_FUNCTION();

		Hazel::Renderer2D::Init();
		m_CheckerboardTexture = Hazel::Texture2D::Create("assets/textures/Checkerboard.png");
		m_ChernoLogoTexture = Hazel::Texture2D::Create("assets/textures/ChernoLogo.png");

	}

	void EditorLayer::OnAttach()
	{
		HZ_PROFILE_FUNCTION();

		Hazel::FramebufferSpecification spec;
		spec.Width = Hazel::Application::Get().GetWindow().GetWidth();
		spec.Height = Hazel::Application::Get().GetWindow().GetHeight();
		m_Framebuffer = Hazel::Framebuffer::Create(spec);

		entt::entity m_EntityHandle = entt::entity();

		m_ActiveScene = CreateRef<Scene>();
		m_SquareEntity = m_ActiveScene->CreateEntity("Green Square");
		m_SquareEntity.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });

		m_SquareEntity = m_ActiveScene->CreateEntity("Red Square");
		m_SquareEntity.AddComponent<SpriteRendererComponent>(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });

		m_PrimaryEntity = m_ActiveScene->CreateEntity("Camera A");
		m_PrimaryEntity.AddComponent<CameraComponent>();

		m_SecondEntity = m_ActiveScene->CreateEntity("Camera B");
		m_SecondEntity.AddComponent<CameraComponent>().Primary = false;

		class CameraController : public ScriptableEntity
		{
		public:
			void OnCreate() override {}
			void OnDestroy() override {}
			void OnUpdate(Timestep ts) override
			{
				auto& translation = GetComponent<TransformComponent>().Translation;
				float speed = 5.0f;

				if (Input::IsKeyPressed(HZ_KEY_A))
					translation.x += speed * ts;
				if(Input::IsKeyPressed(HZ_KEY_D))
					translation.x -= speed * ts;
				if(Input::IsKeyPressed(HZ_KEY_W))
					translation.y -= speed * ts;
				if(Input::IsKeyPressed(HZ_KEY_S))
					translation.y += speed * ts;
			}
		};

		m_PrimaryEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
		m_SecondEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();

		m_SceneHierarchyPanel = SceneHierarchyPanel(m_ActiveScene);
	}

	void EditorLayer::OnUpdate(Hazel::Timestep ts)
	{
		HZ_PROFILE_FUNCTION();

		if (FramebufferSpecification spec = m_Framebuffer->GetSpecifition();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		if(m_ViewportFocused)
			m_CameraController.OnUpdate(ts);

		Hazel::Renderer2D::ResetStat();

		m_Framebuffer->Bind();
		{
			HZ_PROFILE_SCOPE("RenderCommand::Prepare");
			Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
			Hazel::RenderCommand::Clear();
		}

		{
			HZ_PROFILE_SCOPE("Renerer2D::Draw");
			m_ActiveScene->OnUpdate(ts);
		}
		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		HZ_PROFILE_FUNCTION();

		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		style.WindowMinSize.x = minWinSizeX;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows, 
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

				if (ImGui::MenuItem("Exit")) Hazel::Application::Get().Close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}
		m_SceneHierarchyPanel.OnImGuiRender();
		ImGui::Begin("Stats");
		auto stats = Hazel::Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Renderer");
		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
#if 0
		HZ_CORE_WARN("Focused:{0}", m_ViewportFocused);
		HZ_CORE_WARN("Hovered:{0}", m_ViewportHovered);
#endif
		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		auto colorAttachment = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)colorAttachment, ImVec2{ viewportPanelSize.x, viewportPanelSize.y }, ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}

	void EditorLayer::OnEvent(Hazel::Event& event)
	{
		HZ_PROFILE_FUNCTION();

		m_CameraController.OnEvent(event);
	}
}
