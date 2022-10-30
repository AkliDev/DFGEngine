#pragma once
#include "Scene.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

struct Transform
{
	glm::vec3 Translation = { 0.0f,0.0f, 0.0f };
	glm::vec3 Rotation = { 0.0f,0.0f, 0.0f };
	glm::vec3 Scale = { 1.0f,1.0f, 1.0f };

	Transform() = default;
	Transform(const Transform&) = default;
	Transform(const glm::vec3& translatioin)
		: Translation(translatioin) {}

	glm::mat4 GetTransform() const
	{
		glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));
		return glm::translate(glm::mat4(1.0f), Translation) * rotation * glm::scale(glm::mat4(1.0f), Scale);
	}
};

namespace DFGEngine
{
	class Entity
	{
	public:
		Entity(Scene* scene);
		void OnUpdate();
		void OnRender();

		glm::mat4 GetTransform() const { return m_Transform.GetTransform(); }
	private:
		Transform m_Transform;
		Scene* m_Scene = nullptr;
	};
}