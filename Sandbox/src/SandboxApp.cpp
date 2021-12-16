#include <DFGEngine/core/Application.h>
#include <DFGEngine/core/EntryPoint.h>

#include "Sandbox2D.h"

class Sandbox : public DFGEngine::Application
{
public:
	Sandbox()
		: Application("Sandbox")
	{
		PushLayer(new Sandbox2D());
	}

	~Sandbox()
	{
	}
};

DFGEngine::Application* DFGEngine::CreateApplication()
{
	return new Sandbox();
}