#include <Hazel.h>
#include <Hazel/EntryPoint.h>

#include "EditorLayer.h"

namespace Hazel {
	class HazelnutApp : public Application
	{
	public:
		HazelnutApp();
		~HazelnutApp();

	private:

	};

	HazelnutApp::HazelnutApp()
	{
		//PushLayer(new ExampleLayer());
		PushLayer(new EditorLayer());
	}

	HazelnutApp::~HazelnutApp()
	{
	}

	Application* CreateApplication()
	{
		return new HazelnutApp();
	}
}