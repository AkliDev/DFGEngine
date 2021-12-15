#include <DFGEngine/core/Application.h>
#include <DFGEngine/core/EntryPoint.h>

class Sandbox : public DFGEngine::Application
{
public:
	Sandbox()
		: Application("Sandbox")
	{
		
	}

	~Sandbox()
	{
	}
};

DFGEngine::Application* DFGEngine::CreateApplication()
{
	return new Sandbox();
}