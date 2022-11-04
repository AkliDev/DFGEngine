#pragma once
#include "DFGEngine.h"
#include "Entity.h"

namespace DFGEngine
{
	class Ball : public Entity
	{
	public:
		Ball() = default;
		Ball(glm::vec3 pos, float radius, Ref<Texture2D> sprite, glm::vec4 color = glm::vec4(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));

		void Init(Ref<Entity> paddle, float horizontalBound, float verticalBound, float speed);

		void OnUpdate(Timestep ts) override;
		void Reset(glm::vec2 velocity);
		void Stuck(bool stuck) { m_IsStuck = stuck; }

		float GetRadius() const { return m_Radius; }
		bool IsStuck() const { return m_IsStuck; }
		float m_GetBaseSpeed() const { return m_BaseSpeed; }

	private:
		float m_HorizontalBound = 20;
		float m_VerticalBound = 20;
		float m_Radius;
		bool m_IsStuck = true;
		float m_BaseSpeed;
		Ref<Entity> m_Paddle;
	};
}