#include <DFGEngine.h>
#include <DFGEngine/core/EntryPoint.h>

#include "Sandbox2D.h"

namespace DFGEngine
{
	class Sandbox : public Application
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

	Application* DFGEngine::CreateApplication()
	{
		return new Sandbox();
	}
}