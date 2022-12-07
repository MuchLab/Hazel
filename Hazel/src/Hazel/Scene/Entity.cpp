#include "hzpch.h"
#include "Entity.h"

namespace Hazel {
	Entity::Entity(const entt::entity& handle, Scene* scene)
		:m_EntityHandle(handle), m_Scene(scene)
	{
	}
}