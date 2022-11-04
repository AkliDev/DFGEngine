#pragma once
#include "DFGEngine.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace DFGEngine
{
	struct TransformComponent
	{
		glm::vec3 Translation = { 0.0f,0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f,0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f,1.0f, 1.0f };
	
		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translatioin)
			: Translation(translatioin) {}
	
		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));
			return glm::translate(glm::mat4(1.0f), Translation) * rotation * glm::scale(glm::mat4(1.0f), Scale);
		}
	};
	
	struct SpriteRendererComponent
	{
		Ref<Texture2D> Texture;
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		float TilingFactor = 1.0f;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {}
	};

	struct PhysicsComponent
	{
		glm::vec2 Velocity{ 0.0f, 0.0f };

		PhysicsComponent() = default;
		PhysicsComponent(const PhysicsComponent&) = default;
		PhysicsComponent(const glm::vec2& velocity)
			: Velocity(velocity) {}
	};

	class Entity
	{
	public:
		Entity() {};
		Entity(glm::vec3 pos, glm::vec3 scale, Ref<Texture2D> sprite, glm::vec4 color = glm::vec4(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));
		virtual void OnUpdate(Timestep ts);
		virtual void OnRender();

		TransformComponent& GetTransformComponent() { return m_TransformComponent; }
		SpriteRendererComponent& GetSpiteRendererComponent() { return m_SpiteRendererComponent; }
		PhysicsComponent& GetPhysicsComponent() { return m_PhysicsComponent; }
		
	private:
		TransformComponent m_TransformComponent;
		SpriteRendererComponent m_SpiteRendererComponent;
		PhysicsComponent m_PhysicsComponent;
	};
}