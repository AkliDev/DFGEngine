#include "dfgpch.h"
#include "VertexArray.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace DFGEngine
{
	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:	DFG_CORE_ASSERT(false, "RendererAPI::None is currently not supported!") return nullptr;
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLVertexArray>();
		}

		DFG_CORE_ASSERT(false, "Unkown RendererAPI!")
		return nullptr;
	}
}
