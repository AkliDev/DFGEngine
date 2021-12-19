#pragma once

#include "Base.h"

#include "Window.h"
#include "LayerStack.h"
#include "DFGEngine/Events/Event.h"
#include "DFGEngine/Events/ApplicationEvent.h"

#include "DFGEngine/Events/TextEvent.h"
#include "DFGEngine/Events/KeyEvent.h"
#include "DFGEngine/Events/MouseEvent.h"
#include "DFGEngine/Events/ApplicationEvent.h"

namespace DFGEngine
{
	class Application
	{
	public:
		Application(const std::string& name);
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		void Close();

		static Application& Get() { return *s_instance; }	
		inline Window& GetWindow() { return *m_Window; }

	private:
		bool OnWindowClosed(WindowCloseEvent& e);
		bool OnKeyPressedEvent(KeyPressedEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
		bool OnWindowMinimized(WindowMinimizedEvent& e);
		bool OnWindowRestored(WindowRestoredEvent& e);

	private:
		Scope<Window> m_Window;

		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		uint32_t  m_LastFrameTime = 0;

	private:
		static Application* s_instance;
	};

	//To be defined in a CLIENT
	Application* CreateApplication();
}

