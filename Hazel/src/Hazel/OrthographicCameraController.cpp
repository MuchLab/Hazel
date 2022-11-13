#include "hzpch.h"
#include "OrthographicCameraController.h"

#include "Hazel/Input.h"
#include "Hazel/KeyCodes.h"

namespace Hazel {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool isRotation)
		: m_AspectRatio(aspectRatio), m_Camera(-aspectRatio * m_ZoomLevel, aspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_IsRotation(isRotation)
	{

	}

	void OrthographicCameraController::OnUpdate(Timestep& ts)
	{
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
		EventDispatcher dispater(e);
		dispater.Dispatech<MouseScrolledEvent>(HZ_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispater.Dispatech<WindowResizeEvent>(HZ_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));

	}

	// Changed ZoomLevel
	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjectionMatrix(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		
		//m_CameraTranslationSpeed = m_ZoomLevel;
		return false;
	}

	// Changed AspectRatio
	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjectionMatrix(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}
}