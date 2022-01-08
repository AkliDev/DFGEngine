#include "dfgpch.h"

#include "Application.h"

#include "DFGEngine/Renderer/Renderer.h"
#include "DFGEngine/Renderer/Shader.h"
#include "DFGEngine/Core/KeyCodes.h"

#include <SDL.h>
#include <glad/glad.h>

namespace DFGEngine
{
	Application* Application::s_instance = nullptr;
	static Ref<Shader> s_Shader;

	Application::Application(const std::string& name)
	{
		//DFG_PROFILE_FUNCTION();

		DFG_CORE_ASSERT(!s_instance, "Application already exisits!");
		s_instance = this;

		m_Window = Window::Create(WindowProps(name));
		m_Window->SetEventCallback(DFG_BIND_EVENT_FN(Application::OnEvent));
		m_Window->SetVSync(false);

		Renderer::Init();
	}

	Application::~Application()
	{
		//DFG_PROFILE_FUNCTION();

		Renderer::Shutdown();
	}

	void Application::Run()
	{
		const char* vertexShaderSource =
			R"(
			#version 330 core
			layout (location = 0) in vec3 aPosition;
			layout (location = 1) in vec3 aColor;

			out vec3 Color;
			
			void main()
			{
			    gl_Position = vec4(aPosition, 1.0);
				Color = aColor;
			}
		)";

		const char* fragmentShaderSource =
			R"(
			#version 330 core
			out vec4 FragColor;

			in vec3 Color;

			uniform float SinValue;
			
			void main()
			{
			    FragColor = vec4(Color * SinValue, 1.0f);
			}
		)";

		s_Shader = Shader::Create("test", vertexShaderSource, fragmentShaderSource);
#if 0
		unsigned int vertexShader;
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);

		int  success;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			char infoLog[512];
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			DFG_CORE_ERROR("ERROR::SHADER::VERTEX::COMPILATION_FAILED, {0}", infoLog);
		}

		unsigned int fragmentShader;
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			char infoLog[512];
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			DFG_CORE_ERROR("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED, {0}", infoLog);
		}

		unsigned int shaderProgram;
		shaderProgram = glCreateProgram();

		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);

		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success)
		{
			char infoLog[512];
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			DFG_CORE_ERROR("ERROR::SHADER::PROGRAM::LINK_FAILED, {0}", infoLog);
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
#endif

		float vertices[] = {
			// first quad
			 0.5f,  0.5f, 0.0f,	1.0f,  1.0f, 1.0f,	// top right
			 0.5f, -0.5f, 0.0f,	1.0f,  1.0f, 1.0f,	// bottom right
			-0.5f, -0.5f, 0.0f,	1.0f,  1.0f, 1.0f,	// bottom left
			-0.5f,  0.5f, 0.0f,	1.0f,  1.0f, 1.0f,	// top left 
		};

		unsigned int indices[] = {  // note that we start from 0!
			0, 1, 3,   // first triangle
			1, 2, 3    // second triangle
		};

		unsigned int VBO, EBO, VAO;
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		glGenVertexArrays(1, &VAO);
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(VAO);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
		// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
		glBindVertexArray(0);

		//DFG_PROFILE_FUNCTION();
		while (m_Running)
		{
			//DFG_PROFILE_SCOPE("RunLoop");

			uint32_t time = SDL_GetTicks(); // TODO make platform independent 
			float sinValue = sin(time * 0.001f) / 2.0f + 0.5f;

			RenderCommand::SetClearColor({ 0.1, 0.1, 0.1, 1 });
			RenderCommand::Clear();

			// draw our first triangle
			s_Shader->Bind();

			s_Shader->SetFloat("SinValue", sinValue);

			glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			s_Shader->Unbind();

			if (!m_Minimized)
			{			
				//DFG_PROFILE_SCOPE("LayerStack OnUpdate");

				for (Layer* layer : m_LayerStack) { layer->OnUpdate(); }
			}

			m_Window->OnUpdate();
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
