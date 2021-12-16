#pragma once

#include <string>
#include "LayerStack.h"

namespace DFGEngine
{
	class Application
	{
	public:
		Application(const std::string& name);
		virtual ~Application();

		void Run();
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		void Close();

		static Application& Get() { return *s_instance; }	


	private:

	private:

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

