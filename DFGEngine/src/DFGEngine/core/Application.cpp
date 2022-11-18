#include "dfgpch.h"

#include "Application.h"

#include "DFGEngine/Renderer/Renderer.h"
#include "DFGEngine/Audio/SoundEngine.h"
#include "DFGEngine/Core/KeyCodes.h"

#include <SDL.h>

namespace DFGEngine
{
	Application* Application::s_instance = nullptr;

	Application::Application(const std::string& name)
	{
		//DFG_PROFILE_FUNCTION();

		DFG_CORE_ASSERT(!s_instance, "Application already exisits!");
		s_instance = this;

		m_Window = Window::Create(WindowProps(name));
		m_Window->SetEventCallback(DFG_BIND_EVENT_FN(Application::OnEvent));
		m_Window->SetVSync(true);
		m_Window->SetFullScreen(false);
		m_Window->SetShowCursor(false);

		Renderer::Init();
		SoundEngine::Init();
	}

	Application::~Application()
	{
		//DFG_PROFILE_FUNCTION();

		Renderer::Shutdown();
		SoundEngine::Shutdown();
	}

	void Application::Run()
	{
		//DFG_PROFILE_FUNCTION();
		while (m_Running)
		{
			//DFG_PROFILE_SCOPE("RunLoop");
			uint32_t  time = SDL_GetTicks(); // TODO make platform independent 
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			m_Window->OnUpdate();

			if (!m_Minimized)
			{			
				//DFG_PROFILE_SCOPE("LayerStack OnUpdate");

				for (Layer* layer : m_LayerStack) { layer->OnUpdate(timestep); }
				SoundEngine::Update();
			}		
		}
	}

	void Application::OnEvent(Event& e)
	{
		//DFG_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(DFG_BIND_EVENT_FN(Application::OnWindowClosed));
		dispatcher.Dispatch<KeyPressedEvent>(DFG_BIND_EVENT_FN(Application::OnKeyPressedEvent));
		dispatcher.Dispatch<WindowResizeEvent>(DFG_BIND_EVENT_FN(Application::OnWindowResized));
		dispatcher.Dispatch<WindowMinimizedEvent>(DFG_BIND_EVENT_FN(Application::OnWindowMinimized));
		dispatcher.Dispatch<WindowRestoredEvent>(DFG_BIND_EVENT_FN(Application::OnWindowRestored));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);

			if (e.Handled)
			{
				break;
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

	bool Application::OnWindowClosed(WindowCloseEvent& e)
	{
		Close();
		return true;
	}

	bool Application::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		if (e.GetKeyCode() == Key::KEY_F11) { m_Window->SetFullScreen(!m_Window->IsFullScreen()); }
		if (e.GetKeyCode() == Key::KEY_ESCAPE) { Close(); }
		return false;
	}

	bool Application::OnWindowResized(WindowResizeEvent& e)
	{
		//DFG_PROFILE_FUNCTION();

		Renderer::OnWindowResized(e.GetWidth(), e.GetHeight());
		return false;
	}

	bool Application::OnWindowMinimized(WindowMinimizedEvent& e)
	{
		m_Minimized = true;
		return false;
	}

	bool Application::OnWindowRestored(WindowRestoredEvent& e)
	{
		m_Minimized = false;
		return false;
	}

	void Application::Close()
	{
		m_Running = false;
	}
}
