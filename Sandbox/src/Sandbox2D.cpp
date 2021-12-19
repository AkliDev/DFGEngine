#include "Sandbox2D.h"

namespace DFGEngine
{
	Sandbox2D::Sandbox2D()
		:Layer("Sandbox2D")
	{

	}

	void Sandbox2D::OnAttach()
	{
		DFG_TRACE("Lets do this quick and dirty!");
	}

	void Sandbox2D::OnDetach()
	{

	}

	void Sandbox2D::OnUpdate()
	{
		RenderCommand::SetClearColor({ 0.1, 0.1, 0.1, 1 });
		RenderCommand::Clear();
	}

	void Sandbox2D::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		DFG_TRACE(e);
	}
}