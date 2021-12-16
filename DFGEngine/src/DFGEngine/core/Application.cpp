#include "dfgpch.h"

#include "Application.h"
//#include <SDL.h>

namespace DFGEngine
{
	Application* Application::s_instance = nullptr;

	Application::Application(const std::string& name)
	{

	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		std::cout << "Lets do this quick and dirty!" << std::endl;
		std::cin.get();
	}

	void Application::Close()
	{
		m_Running = false;
	}
}
