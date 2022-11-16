#include <Hazel.h>
#include <Hazel/EntryPoint.h>

#include "Layer/ExampleLayer.h"
#include "Layer/Sandbox2DLayer.h"

class Sandbox : public Hazel::Application
{
public:
	Sandbox();
	~Sandbox();

private:

};

Sandbox::Sandbox()
{
	//PushLayer(new ExampleLayer());
	PushLayer(new Sandbox2DLayer());
}

Sandbox::~Sandbox()
{
}

Hazel::Application* Hazel::CreateApplication()
{
	return new Sandbox();
}