#pragma once
#include <entt.hpp>

#include "Hazel/Core/Timestep.h"

namespace Hazel {

	class Scene {
	public:
		Scene();
		~Scene();

		void OnUpdate(Timestep ts);
		entt::registry& Reg() { return m_Registry; }
	private:
		entt::registry m_Registry;
	};
}