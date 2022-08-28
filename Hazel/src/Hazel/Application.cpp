#include "hzpch.h"

#include "Application.h"
#include "Event/ApplicationEvent.h"
#include "Event/MouseEvent.h"
#include "Log.h"
namespace Hazel{
	Application::Application()
	{
	}
	Application::~Application()
	{
	}
	void Application::Run()
	{
		WindowResizeEvent e1(1000, 2000);
		HZ_TRACE(e1.ToString());

		MouseButtonPressedEvent e2(10);
		HZ_TRACE(e2.ToString());
	}
}