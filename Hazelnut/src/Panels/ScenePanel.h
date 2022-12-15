#pragma once
#include <Hazel.h>

namespace Hazel {
	class ScenePanel {
	public:
		ScenePanel() = default;
		ScenePanel(Ref<Scene>& scene);

		void SetScenePanel(Ref<Scene>& scene) { m_Scene = scene; }
		void OnImGuiRender();
	private:
		Ref<Scene> m_Scene;
		Entity m_SeletedContext;
	};
}