#include "dfgpch.h"
#include "Renderer.h"

#include "DFGEngine/Renderer/Renderer2D.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace DFGEngine
{
	void Renderer::Init()
	{
		//DFG_PROFILE_FUNCTION();
		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::Shutdown()
	{
		Renderer2D::ShutDown();
	}

	void Renderer::OnWindowResized(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}
}
