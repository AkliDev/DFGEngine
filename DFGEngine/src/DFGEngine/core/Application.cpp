#include "dfgpch.h"

#include "Application.h"
#include <SDL.h>

namespace DFGEngine
{
	Application* Application::s_instance = nullptr;

	Application::Application(const std::string& name)
	{
		s_instance = this;
		int success = SDL_Init(SDL_INIT_VIDEO);
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		while (m_Running)
		{
			if (!m_Minimized)
			{			
				for (Layer* layer : m_LayerStack) { layer->OnUpdate(); }
			}
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::Close()
	{
		m_Running = false;
	}
}
