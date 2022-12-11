#pragma once

#include <glm/glm.hpp>

namespace DFGEngine::Math 
{
	bool DecomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);
	float lerp(float a, float b, float t);
}
