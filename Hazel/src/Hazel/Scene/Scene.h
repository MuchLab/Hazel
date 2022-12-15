#pragma once
#include <entt.hpp>
#include <string>
#include "Hazel/Core/Timestep.h"

namespace Hazel {
	class Entity;
	class Scene {
	public:
		Scene();
		~Scene();

		void OnUpdate(Timestep ts);
		void OnViewportResize(uint32_t width, uint32_t height);
		Entity CreateEntity(const std::string& tag = "Entity");
	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
		friend class Entity;
		friend class ScenePanel;
	};
}