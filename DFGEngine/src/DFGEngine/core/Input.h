#pragma once

#include <glm/glm.hpp>

#include "DFGEngine/Core/Base.h"
#include "DFGEngine/Core/KeyCodes.h"
#include "DFGEngine/Core/MouseCodes.h"

namespace DFGEngine
{
	class  Input
	{
	public:
		static bool IsKeyPressed(KeyCode key);

		static bool IsMouseButtonPressed(MouseCode button);
		static glm::vec2 GetMousePosition();
		static float  GetMouseX();
		static float GetMouseY();
	};
}
