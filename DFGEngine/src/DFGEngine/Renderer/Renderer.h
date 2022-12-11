#pragma once

#include "RenderCommand.h"

namespace DFGEngine
{
	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void OnWindowResized(uint32_t width, uint32_t height);

		static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};
}