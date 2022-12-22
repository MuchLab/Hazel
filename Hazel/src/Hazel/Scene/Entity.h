#pragma once

#include "Scene.h"
#include "entt.hpp"

namespace Hazel {
	class Entity {
	public:
		Entity() = default;
		Entity(const entt::entity& handle, Scene* scene);
		~Entity() = default;

		template<typename T>
		T& GetComponent()
		{
			HZ_CORE_ASSERT(HasComponent<T>(), "entity does not have this component.");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T, typename ...Args>
		T& AddComponent(Args&& ...args) 
		{
			HZ_CORE_ASSERT(!HasComponent<T>(), "entity already has this component.");
			T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded<T>(*this, component);
			return component;
		}
		
		template<typename T>
		void RemoveComponent()
		{
			HZ_CORE_ASSERT(HasComponent<T>(), "entity does not have this component.");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		template<typename T>
		bool HasComponent() 
		{
			return m_Scene->m_Registry.has<T>(m_EntityHandle);
		}

		operator bool() const { return m_EntityHandle != entt::null; }
		operator entt::entity() const { return m_EntityHandle; }
		bool operator==(const Entity& other) const { return other.m_EntityHandle == m_EntityHandle && other.m_Scene == m_Scene; }
		bool operator!=(const Entity& other) const { return other != *this; }
	private:
		entt::entity m_EntityHandle{ entt::null };
		Scene* m_Scene = nullptr;
	};
}
