#include <iostream>
#include "Sandbox2D.h"

Sandbox2D::Sandbox2D()
	:Layer("Sandbox2D")
{
	
}

void Sandbox2D::OnAttach()
{
	std::cout << "Lets do this quick and dirty!" << std::endl;
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate()
{

}