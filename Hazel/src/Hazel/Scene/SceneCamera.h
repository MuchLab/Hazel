#pragma once
#include "Hazel/Renderer/Camera.h"

namespace Hazel {
	class SceneCamera : public Camera
	{
	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1 };
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetPerspective(float verticalFOV, float nearClip, float farClip);
		void SetOrthographic(float size, float nearClip, float farClip);

		void SetViewportSize(uint32_t width, uint32_t height);

		float GetPerspectiveFov() const { return m_PerspectiveFov; }
		void SetPerspectiveFov(float fov) { m_PerspectiveFov = fov; RecalculateProjection(); }
		float GetPerspectiveNear() const { return m_PerspectiveNear; }
		void SetPerspectiveNear(float p_Near) { m_PerspectiveNear = p_Near; RecalculateProjection(); }
		float GetPerspectiveFar() const { return m_PerspectiveFar; }
		void SetPerspectiveFar(float p_Far) { m_PerspectiveFar = p_Far; RecalculateProjection(); }

		float GetOrthographicSize() const { return m_OrthographicSize; }
		void SetOrthographicSize(float size) { m_OrthographicSize = size; RecalculateProjection(); }
		float GetOrthographicNear() const { return m_OrthographicNear; }
		void SetOrthographicNear(float o_Near) { m_OrthographicNear = o_Near; RecalculateProjection(); }
		float GetOrthographicFar() const { return m_OrthographicFar; }
		void SetOrthographicFar(float o_Far) { m_OrthographicFar = o_Far; RecalculateProjection(); }

		ProjectionType GetProjectionType() const { return m_ProjectionType; }
		void SetProjectionType(ProjectionType projectionType) { m_ProjectionType = projectionType; }

	private:
		void RecalculateProjection();
	private:
		ProjectionType m_ProjectionType = ProjectionType::Orthographic;

		float m_PerspectiveFov = glm::radians(45.0f);
		float m_PerspectiveNear = 0.01f, m_PerspectiveFar = 1000.0f;

		float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;

		float m_AspectRatio = 0.0f;
	};
}
