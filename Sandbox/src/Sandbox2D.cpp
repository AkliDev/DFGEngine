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
		
	}

	void Sandbox2D::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
	}
}