#include <Hazel.h>
#include "imGui/imgui.h"

class ExampleLayer : public Hazel::Layer {
public:
	ExampleLayer()
		: Layer("Example") 
	{
	}

	void OnUpdate() override
	{
		if (Hazel::Input::IsKeyPressed(HZ_KEY_TAB))
			HZ_TRACE("Tab key is pressed!(poll)");
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("TEST");
		ImGui::Text("HelloWorld.");
		ImGui::End();
	}


	void OnEvent(Hazel::Event& event) override
	{
		if (event.GetEventType() == Hazel::EventType::KeyPressed)
		{
			Hazel::KeyPressedEvent& e = (Hazel::KeyPressedEvent&)event;
			if(e.GetKeyCode() == HZ_KEY_TAB)
				HZ_TRACE("Tab key is pressed!(event)");
			//HZ_TRACE("{0}", (char)e.GetKeyCode());
		}
	}
};



class Sandbox : public Hazel::Application
{
public:
	Sandbox();
	~Sandbox();

private:

};

Sandbox::Sandbox()
{
	PushLayer(new ExampleLayer());
}

Sandbox::~Sandbox()
{
}

Hazel::Application* Hazel::CreateApplication()
{
	return new Sandbox();
}