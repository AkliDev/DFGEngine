#pragma once

extern DFGEngine::Application* DFGEngine::CreateApplication(ApplicationCommandLineArgs args);
int main(int argc, char** argv)
{	
	DFGEngine::Log::Init();

	auto app = DFGEngine::CreateApplication({ argc, argv });
	
	app->Run();
	
	delete app;
}