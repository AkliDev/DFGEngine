#include "Utility.h"

namespace DFGEngine
{
	// AABB - AABB collision
	bool CheckCollision(Ref<Entity> a, Ref<Entity> b) 
	{		
		TransformComponent& a_TC = a->GetTransformComponent();
		TransformComponent& b_TC = b->GetTransformComponent();

		//if we are not colliding
		bool isNotOverlapping = a_TC.Translation.x - a_TC.Scale.x * 0.5f > b_TC.Translation.x + b_TC.Scale.x * 0.5f ||
								b_TC.Translation.x - b_TC.Scale.x * 0.5f > a_TC.Translation.x + a_TC.Scale.x * 0.5f ||
								a_TC.Translation.y - a_TC.Scale.y * 0.5f > b_TC.Translation.y + b_TC.Scale.y * 0.5f ||
								b_TC.Translation.y - b_TC.Scale.y * 0.5f > a_TC.Translation.y + a_TC.Scale.y * 0.5f;
		
		// Return opposite result
		return false;
	}

	Collision CheckCollision(Ref<Ball>a, Ref<Entity> b)
	{
		TransformComponent& a_TC = a->GetTransformComponent();
		TransformComponent& b_TC = b->GetTransformComponent();

		// get difference vector between both centers
		glm::vec3 difference = a_TC.Translation - b_TC.Translation;
		glm::vec3 clamped = glm::clamp(difference, -b_TC.Scale * 0.5f, b_TC.Scale * 0.5f);

		// add clamped value to AABB_center and we get the value of box closest to circle
		glm::vec3 closest = b_TC.Translation + clamped;
		// retrieve vector between center circle and closest point AABB and check if length <= radius
		difference = closest - a_TC.Translation;

		if (glm::length(difference) <= a->GetRadius())
		{
			return std::make_tuple(true, VectorDirection(difference), difference);
		}
		else
		{
			return std::make_tuple(false, UP, glm::vec2(0.0f, 0.0f));
		}
	}

	Direction VectorDirection(glm::vec2 target)
	{
		glm::vec2 compass[] = {
			glm::vec2(0.0f, 1.0f),	// up
			glm::vec2(0.0f, -1.0f),	// down
			glm::vec2(-1.0f, 0.0f),	// left
			glm::vec2(1.0f, 0.0f)	// right	
		};
		float max = 0.0f;
		unsigned int best_match = -1;
		for (unsigned int i = 0; i < 4; i++)
		{
			float dot_product = glm::dot(glm::normalize(target), compass[i]);
			if (dot_product > max)
			{
				max = dot_product;
				best_match = i;
			}
		}
		return (Direction)best_match;
	}
}