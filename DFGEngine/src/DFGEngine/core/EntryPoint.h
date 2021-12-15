#pragma once

extern DFGEngine::Application* DFGEngine::CreateApplication();
int main(int argc, char** argv)
{	
	auto app = DFGEngine::CreateApplication();
	
	app->Run();
	
	delete app;
}