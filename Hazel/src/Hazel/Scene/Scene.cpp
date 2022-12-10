#include "hzpch.h"
#include "Scene.h"
#include "Components.h"
#include "Hazel/Renderer/Renderer2D.h"
#include "Entity.h"

namespace Hazel {

	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	void Scene::OnUpdate(Timestep ts)
	{
		Camera* mainCamera = nullptr;
		glm::mat4* mainTransform = nullptr;
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto& entity : view)
			{
				auto& [transform, camera] = m_Registry.get<TransformComponent, CameraComponent>(entity);
				if (camera.Primary) 
				{ 
					mainCamera = &camera.Camera; 
					mainTransform = &transform.Transform; 
					break;
				}
			}
		}
		if (mainCamera) 
		{
			Renderer2D::BeginScene(*mainCamera, *mainTransform);
			auto group = m_Registry.group<TransformComponent, SpriteRendererComponent>();
			for (auto& entity : group)
			{
				auto& [transform, sprite] = m_Registry.get<TransformComponent, SpriteRendererComponent>(entity);
				Renderer2D::DrawQuad(transform, sprite.Color);
			}
			Renderer2D::EndScene();
		}
	}

	Entity Scene::CreateEntity(const std::string& tag)
	{
		Entity entity = { m_Registry.create(), this };

		entity.AddComponent<TagComponent>(tag);
		entity.AddComponent<TransformComponent>();

		return entity;
	}

}
