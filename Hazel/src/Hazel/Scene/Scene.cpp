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
		{
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
			{
				if (!nsc.Instance)
				{
					nsc.Instance = nsc.InstantiateScript();
					nsc.Instance->m_Entity = Entity{ entity, this };
					nsc.Instance->OnCreate();
				}
				nsc.Instance->OnUpdate(ts);
			});
		}

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
					mainTransform = &transform.GetTransform(); 
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
				Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
			}
			Renderer2D::EndScene();
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		auto view = m_Registry.view<CameraComponent>();
		for (auto& entity : view)
		{
			auto& camera = view.get<CameraComponent>(entity);
			if (!camera.FixedAspectRatio)
				camera.Camera.SetViewportSize(width, height);
		}
	}

	Entity Scene::CreateEntity(const std::string& tag)
	{
		Entity entity = { m_Registry.create(), this };

		entity.AddComponent<TagComponent>(tag);
		entity.AddComponent<TransformComponent>();

		return entity;
	}

	void Scene::DestroyEntity(Entity& entity)
	{
		m_Registry.destroy(entity);
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(false);
	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{
	}

}
