#pragma once
#include "Hazel/Core.h"
#include "Hazel/Layer.h"
#include "Hazel/Event/ApplicationEvent.h"
#include "Hazel/Event/KeyEvent.h"
#include "Hazel/Event/MouseEvent.h"

namespace Hazel {
	class HAZEL_API ImGuiLayer : public Layer
	{
	public:

		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(Event& event);
		
	private:

		bool OnKeyPress(KeyPressedEvent& e);
		bool OnKeyRelease(KeyReleasedEvent& e);
		bool OnMouseMove(MouseMovedEvent& e);
		bool OnMousePresse(MouseButtonPressedEvent& e);
		bool OnMouseRelease(MouseButtonReleasedEvent& e);
		bool OnMouseScroll(MouseScrolledEvent& e);
		bool OnKeyType(KeyTypedEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);


	private:
		float m_Time = 0.0f;

	};
}

