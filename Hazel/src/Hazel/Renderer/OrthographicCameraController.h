#pragma once

#include "Hazel/Renderer/OrthographicCamera.h"
#include "Hazel/Core/Timestep.h"
#include "Hazel/Event/MouseEvent.h"
#include "Hazel/Event/ApplicationEvent.h"

namespace Hazel {

	struct OrthographicCameraBounds
	{
		float Left, Right;
		float Bottom, Top;

		float GetWidth() { return Right - Left; }
		float GetHeight() { return Top - Bottom; }
	};

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float zoomLevel, float aspectRatio, bool isRotation = false);

		void OnUpdate(Timestep& ts);
		void OnEvent(Event& e);
		void OnResize(float width, float height);

		inline OrthographicCamera& GetCamera() { return m_Camera; }
		inline const OrthographicCamera& GetCamera() const { return m_Camera; }

		inline const OrthographicCameraBounds& GetBounds() const { return m_Bounds; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

	private:
		float m_ZoomLevel;
		float m_AspectRatio;
		OrthographicCameraBounds m_Bounds;
		OrthographicCamera m_Camera;
		bool m_IsRotation;
		

		glm::vec3 m_CameraPosition = glm::vec3(0.0f);
		float m_CameraTranslationSpeed = 5.0f;

		float m_CameraRotation = 0.0f;
		float m_CameraRotateSpeed = 180.0f;
	};

}