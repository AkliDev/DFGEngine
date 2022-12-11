#pragma once
#include "DFGEngine.h"
#include "Entity.h"
#include "Ball.h"
#include <tuple>

namespace DFGEngine
{
	enum Direction
	{
		UP,
		DOWN,
		LEFT,
		RIGHT
	};

	typedef std::tuple<bool, Direction, glm::vec2> Collision;

	// AABB
	bool CheckCollision(Ref<Entity> a, Ref<Entity> b);
	// AABB - Circle collision
	Collision CheckCollision(Ref<Ball> a, Ref<Entity> b);

	Direction VectorDirection(glm::vec2 target);
}