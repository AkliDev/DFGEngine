#include "dfgpch.h"

#include "Application.h"

#include "DFGEngine/Renderer/Renderer.h"
#include "DFGEngine/Audio/SoundEngine.h"
#include "DFGEngine/Core/KeyCodes.h"

#include "DFGEngine/Utils/PlatformUtils.h"

namespace DFGEngine
{
	const int SCREEN_FPS = 60;
	const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;

	Application* Application::s_instance = nullptr;

	Application::Application(const ApplicationSpecification& specification)
		: m_Specification(specification)
	{
		//DFG_PROFILE_FUNCTION();

		DFG_CORE_ASSERT(!s_instance, "Application already exisits!");
		s_instance = this;

		m_Window = Window::Create(WindowProps(m_Specification.Name));
		m_Window->SetEventCallback(DFG_BIND_EVENT_FN(Application::OnEvent));
		m_Window->SetVSync(false);		
		m_Window->SetShowCursor(false);
		#ifdef DFG_RELEASE
		m_Window->SetFullScreen(true);
		#endif

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
		LTimer fpsTimer;
		LTimer capTimer;
		fpsTimer.start();
		uint32_t lastFrameTime = 0.0f;
		uint32_t countedFrames = 0;

		while (m_Running)
		{
			//Calculate and correct fps
			float avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);
			if (avgFPS > 2000000)
			{
				avgFPS = 0;
			}

			//DFG_PROFILE_SCOPE("RunLoop");
			uint32_t time = Time::GetTime();
			Timestep timestep = time - lastFrameTime;
			lastFrameTime = time;

			m_Window->OnUpdate();

			//if (!m_Minimized)
			{
				//DFG_PROFILE_SCOPE("LayerStack OnUpdate");

				for (Layer* layer : m_LayerStack) { layer->OnUpdate(timestep); }
				SoundEngine::Update();
				countedFrames++;
			}

			//If frame finished early
			int frameTicks = capTimer.getTicks();
			if (frameTicks < SCREEN_TICK_PER_FRAME)
			{
				//Wait remaining time
				std::this_thread::sleep_for(std::chrono::milliseconds(SCREEN_TICK_PER_FRAME - frameTicks));
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