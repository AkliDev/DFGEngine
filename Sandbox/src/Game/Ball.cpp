#include "Ball.h"

namespace DFGEngine
{
	Ball::Ball(glm::vec3 pos, float radius, Ref<Texture2D> sprite, glm::vec4 color, glm::vec2 velocity)
		:Entity(pos, glm::vec3(radius * 2.0f), sprite, color, velocity),
		m_Radius(radius)
	{

	}

	void Ball::Init(Ref<Entity> paddle, float horizontalBound, float verticalBound, float speed)
	{
		m_Paddle = paddle;
		m_HorizontalBound = horizontalBound;
		m_VerticalBound = verticalBound;

		m_BaseSpeed = speed;
		GetPhysicsComponent().Velocity.y = m_BaseSpeed;
		GetPhysicsComponent().Velocity.x = m_BaseSpeed;
	}

	void Ball::OnUpdate(Timestep ts)
	{		
		if (m_IsStuck == false)
		{
			Entity::OnUpdate(ts);
			// Check bounds and make the ball BOUNCE <<< That was the theme. Yes not very creative I know.
			if (GetTransformComponent().Translation.x - m_Radius < 0.0f)
			{
				GetPhysicsComponent().Velocity.x *= -1;
				GetTransformComponent().Translation.x = m_Radius;
			}
			else if (GetTransformComponent().Translation.x + m_Radius > m_HorizontalBound)
			{
				GetPhysicsComponent().Velocity.x *= -1;
				GetTransformComponent().Translation.x = m_HorizontalBound - m_Radius;
			}
			
			if (GetTransformComponent().Translation.y + m_Radius > m_VerticalBound)
			{
				GetPhysicsComponent().Velocity.y = -GetPhysicsComponent().Velocity.y;
				GetTransformComponent().Translation.y = m_VerticalBound -m_Radius;
			}
		}
		else
		{
			glm::vec3 pos = m_Paddle->GetTransformComponent().Translation;
			pos.y += (m_Paddle->GetTransformComponent().Scale.y * 0.5f) + (m_Radius);
			GetTransformComponent().Translation = pos;
		}
	}

	void Ball::Reset(glm::vec2 velocity)
	{
		GetPhysicsComponent().Velocity = velocity;
		m_IsStuck = true;
	}
}