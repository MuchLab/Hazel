#include "hzpch.h"
#include "OrthographicCameraController.h"

#include "Hazel/Core/Input.h"
#include "Hazel/Core/KeyCodes.h"

namespace Hazel {

	OrthographicCameraController::OrthographicCameraController(float zoomLevel, float aspectRatio, bool isRotation)
		: m_ZoomLevel(zoomLevel),
		m_AspectRatio(aspectRatio), 
		m_Bounds({ -aspectRatio * m_ZoomLevel, aspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel }),
		m_Camera(-aspectRatio * m_ZoomLevel, aspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), 
		m_IsRotation(isRotation)
	{
		HZ_PROFILE_FUNCTION();
	}

	void OrthographicCameraController::OnUpdate(Timestep& ts)
	{
		HZ_PROFILE_FUNCTION();

		if (Input::IsKeyPressed(HZ_KEY_D))
			m_CameraPosition.x -= m_CameraTranslationSpeed * m_ZoomLevel * ts;
		else if (Input::IsKeyPressed(HZ_KEY_A))
			m_CameraPosition.x += m_CameraTranslationSpeed * m_ZoomLevel * ts;
		if (Input::IsKeyPressed(HZ_KEY_W))
			m_CameraPosition.y -= m_CameraTranslationSpeed * m_ZoomLevel * ts;
		else if (Input::IsKeyPressed(HZ_KEY_S))
			m_CameraPosition.y += m_CameraTranslationSpeed * m_ZoomLevel * ts;

		if (m_IsRotation)
		{
			if (Input::IsKeyPressed(HZ_KEY_Q))
				m_CameraRotation -= m_CameraRotateSpeed * ts;
			else if (Input::IsKeyPressed(HZ_KEY_E))
				m_CameraRotation += m_CameraRotateSpeed * ts;
		}
		
		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		HZ_PROFILE_FUNCTION();

		EventDispatcher dispater(e);
		dispater.Dispatech<MouseScrolledEvent>(HZ_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispater.Dispatech<WindowResizeEvent>(HZ_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));

	}

	void OrthographicCameraController::OnResize(float width, float height)
	{
		m_AspectRatio = width / height;
		m_Bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
		m_Camera.SetProjectionMatrix(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top);
	}

	// Changed ZoomLevel
	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		HZ_PROFILE_FUNCTION();

		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
		m_Camera.SetProjectionMatrix(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top);
		
		//m_CameraTranslationSpeed = m_ZoomLevel;
		return false;
	}

	// Changed AspectRatio
	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		HZ_PROFILE_FUNCTION();

		OnResize(e.GetWidth(), e.GetHeight());
		return false;
	}
}