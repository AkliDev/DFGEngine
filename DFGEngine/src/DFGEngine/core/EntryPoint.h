#pragma once

extern DFGEngine::Application* DFGEngine::CreateApplication();
int main(int argc, char** argv)
{	
	DFGEngine::Log::Init();

	auto app = DFGEngine::CreateApplication();
	
	app->Run();
	
	delete app;
}