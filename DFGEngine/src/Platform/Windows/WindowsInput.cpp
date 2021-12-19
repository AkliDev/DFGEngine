#include "dfgpch.h"
#include "DFGEngine/Core/Input.h"

#include <SDL.h>

namespace DFGEngine
{
	bool Input::IsKeyPressed(const KeyCode key)
	{
		//auto window = static_cast<SDL_Window*>(Application::Get().GetWindow().GetNativeWindow());
		const Uint8* state = SDL_GetKeyboardState(NULL);
		return state[key];
	}

	bool Input::IsMouseButtonPressed(const MouseCode button)
	{
		return SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(button);
	}

	glm::vec2 Input::GetMousePosition()
	{
		int x, y;
		Uint32 state = SDL_GetMouseState(&x, &y);
		return {(float)x,(float)y};
	}

	float Input::GetMouseX()
	{
		return GetMousePosition().x;
	}

	float Input::GetMouseY()
	{
		return GetMousePosition().y;
	}
}
