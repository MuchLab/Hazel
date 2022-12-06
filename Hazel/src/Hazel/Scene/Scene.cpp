#include "hzpch.h"
#include "Scene.h"
#include "Components.h"
#include "Hazel/Renderer/Renderer2D.h"

namespace Hazel {

	Scene::Scene()
	{
		
	}

	Scene::~Scene()
	{
	}

	void Scene::OnUpdate(Timestep ts)
	{
		auto& group = m_Registry.group<TransformComponent, SpriteRendererComponent>();
		for (auto& entity : group)
		{
			auto&[transform, sprite] = m_Registry.get<TransformComponent, SpriteRendererComponent>(entity);
			Renderer2D::DrawQuad(transform, sprite.Color);
		}
	}

}
