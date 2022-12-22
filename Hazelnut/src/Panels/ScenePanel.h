#pragma once
#include <Hazel.h>

namespace Hazel {
	class ScenePanel {
	public:
		ScenePanel() = default;
		ScenePanel(Ref<Scene>& scene);

		void SetScenePanel(Ref<Scene>& scene) { m_Scene = scene; }
		void OnImGuiRender();
		void DrawTreeNodes();
		void DrawComponents();
	private:
		static void DrawDragFloat3(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f);
	private:
		Ref<Scene> m_Scene;
		Entity m_SelectedContext;
	};
}