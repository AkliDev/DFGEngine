#include "dfgpch.h"

#include "FrameBuffer.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLFrameBuffer.h"

namespace DFGEngine
{
	Ref<FrameBuffer> FrameBuffer::Create(const FramebufferSpecification& specification)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:	DFG_CORE_ASSERT(false, "RendererAPI::None is currently not supported!") return nullptr;
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLFrameBuffer>(specification);
		}

		DFG_CORE_ASSERT(false, "Unkown RendererAPI!")
		return nullptr;
	}
}
