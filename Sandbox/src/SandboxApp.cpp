#include <DFGEngine.h>
#include <DFGEngine/core/EntryPoint.h>

#include "Sandbox2D.h"

namespace DFGEngine
{
	class Sandbox : public Application
	{
	public:
		Sandbox(const ApplicationSpecification& spec)
			: Application(spec)
		{
			PushLayer(new Sandbox2D());
		}

		~Sandbox()
		{
		}
	};

	Application* CreateApplication(ApplicationCommandLineArgs args)
	{
		ApplicationSpecification spec;
		spec.Name = "Rollback Test";
		spec.CommandLineArgs = args;

		return new Sandbox(spec);
	}
}