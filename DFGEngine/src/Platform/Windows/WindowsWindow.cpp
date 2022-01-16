#include "dfgpch.h"

#include "WindowsWindow.h"

#include "DFGEngine/Events/ApplicationEvent.h"
#include "DFGEngine/Events/KeyEvent.h"
#include "DFGEngine/Events/TextEvent.h"
#include "DFGEngine/Events/MouseEvent.h"

#include "DFGEngine/Renderer/Renderer.h"
#include "DFGEngine/Renderer/GraphicsContext.h"

namespace DFGEngine {

	static uint8_t s_SDLWindowCount = 0;

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		//DFG_PROFILE_FUNCTION();

		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		//DFG_PROFILE_FUNCTION();

		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		DFG_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (s_SDLWindowCount == 0)
		{
			//DFG_PROFILE_SCOPE("SDL_Init");

			DFG_CORE_INFO("Initializing SDL");
			int success = SDL_Init(SDL_INIT_VIDEO);
			if (success != 0)
			{
				DFG_CORE_ASSERT(success, "Could not initialize SDL!");
			}
		}

		{
			//DFG_PROFILE_SCOPE("SDLCreateWindow");
			m_Window = SDL_CreateWindow(props.Title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, props.Width, props.Height, SDL_WINDOW_OPENGL);		
			SDL_SetWindowResizable(m_Window, SDL_TRUE);
			++s_SDLWindowCount;
		}

		m_Context = GraphicsContext::Create(m_Window);
		m_Context->Init();

		SDL_SetWindowData(m_Window, "data", &m_Data);
		SetVSync(false);
	}

	void WindowsWindow::Shutdown()
	{
		//DFG_PROFILE_FUNCTION();

		m_Context->DeleteRenderContext();
		SDL_DestroyWindow(m_Window);
		s_SDLWindowCount--;

		if (s_SDLWindowCount == 0)
		{
			SDL_Quit();
		}
	}

	void WindowsWindow::OnUpdate()
	{
		//DFG_PROFILE_FUNCTION();

		HandleEvents();
		m_Context->SwapBuffer();
	}

	void WindowsWindow::HandleEvents()
	{
		SDL_Event event;

		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_WINDOWEVENT:
			{
				if (event.window.windowID == SDL_GetWindowID(m_Window))
				{
					switch (event.window.event)
					{
					case SDL_WINDOWEVENT_SIZE_CHANGED:
					{
						int width = (int)event.window.data1;
						int height = (int)event.window.data2;

						WindowData& data = *(WindowData*)SDL_GetWindowData(m_Window, "data");
						data.Width = width;
						data.Height = height;

						WindowResizeEvent DFG_event(width, height);
						data.EventCallback(DFG_event);

						break;
					}
					case SDL_WINDOWEVENT_CLOSE:
					{
						WindowData& data = *(WindowData*)SDL_GetWindowData(m_Window, "data");
						WindowCloseEvent DFG_event;
						data.EventCallback(DFG_event);
						break;
					}
					case SDL_WINDOWEVENT_MINIMIZED:
					{
						WindowData& data = *(WindowData*)SDL_GetWindowData(m_Window, "data");
						WindowMinimizedEvent DFG_event;
						data.EventCallback(DFG_event);
						break;
					}
					case SDL_WINDOWEVENT_RESTORED:
					{
						WindowData& data = *(WindowData*)SDL_GetWindowData(m_Window, "data");
						WindowRestoredEvent DFG_event;
						data.EventCallback(DFG_event);
						break;
					}
					}
				}
				break;
			}
			case SDL_TEXTINPUT:
			{
				WindowData& data = *(WindowData*)SDL_GetWindowData(m_Window, "data");
				TextInputEvent DFG_event(event.text.text);
				data.EventCallback(DFG_event);
				break;
			}
			case SDL_KEYDOWN:
			{

				WindowData& data = *(WindowData*)SDL_GetWindowData(m_Window, "data");

				if (event.key.repeat != 0)
				{
					KeyPressedEvent DFG_event(event.key.keysym.scancode, event.key.repeat);
					data.EventCallback(DFG_event);
				}
				else
				{
					KeyPressedEvent DFG_event(event.key.keysym.scancode, 0);
					data.EventCallback(DFG_event);
				}

				KeyTypedEvent DFG_event(event.key.keysym.sym);
				data.EventCallback(DFG_event);
				break;
			}
			case SDL_KEYUP:
			{
				WindowData& data = *(WindowData*)SDL_GetWindowData(m_Window, "data");
				KeyReleasedEvent DFG_event(event.key.keysym.scancode);
				data.EventCallback(DFG_event);
				break;
			}
			case SDL_MOUSEBUTTONDOWN:
			{
				WindowData& data = *(WindowData*)SDL_GetWindowData(m_Window, "data");
				MouseButtonPressedEvent DFG_event(event.button.button);
				data.EventCallback(DFG_event);
				break;
			}
			case SDL_MOUSEBUTTONUP:
			{
				WindowData& data = *(WindowData*)SDL_GetWindowData(m_Window, "data");
				MouseButtonReleasedEvent DFG_event(event.button.button);
				data.EventCallback(DFG_event);
				break;
			}
			case SDL_MOUSEWHEEL:
			{
				WindowData& data = *(WindowData*)SDL_GetWindowData(m_Window, "data");
				MouseScrolledEvent DFG_event(float(event.wheel.x), float(event.wheel.y));
				data.EventCallback(DFG_event);
				break;
			}
			case SDL_MOUSEMOTION:
			{
				WindowData& data = *(WindowData*)SDL_GetWindowData(m_Window, "data");
				MouseMovedEvent DFG_event(float(event.motion.x), float(event.motion.y));
				data.EventCallback(DFG_event);
				break;
			}
			}
		}
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		//DFG_PROFILE_FUNCTION();

		if (enabled)
			SDL_GL_SetSwapInterval(1);
		else
			SDL_GL_SetSwapInterval(0);

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}
}
