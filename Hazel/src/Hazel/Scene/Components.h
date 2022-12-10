#pragma once
#include <glm/glm.hpp>

#include "Hazel/Renderer/Camera.h"

namespace Hazel {

	struct TagComponent {
		std::string Tag = std::string();

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}

		operator std::string& () { return Tag; }
	};

	struct TransformComponent {
		glm::mat4 Transform{ 1.0f };
		
		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& transform)
			: Transform(transform) {}

		operator glm::mat4& () { return Transform; }
	};

	struct SpriteRendererComponent {
		glm::vec4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {}
	};

	struct CameraComponent {
		Hazel::Camera Camera;
		bool Primary = true;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(const glm::mat4& projection)
			: Camera(projection) {}
		CameraComponent(const glm::mat4& projection, const bool primary)
			: Camera(projection), Primary(primary) {}
	};
}
