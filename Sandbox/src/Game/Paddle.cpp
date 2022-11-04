#include "Paddle.h"

namespace DFGEngine
{
	Paddle::Paddle(glm::vec3 pos, glm::vec3 scale, Ref<Texture2D> sprite, glm::vec4 color, glm::vec2 velocity)
		:Entity(pos, scale, sprite, color, velocity),
		m_BaseScale(scale)
	{

	}

	void Paddle::Init(float horizonatalBound, float speed)
	{
		m_HorizontalBound = horizonatalBound;
		m_BaseSpeed = speed;
		m_CurrentSpeed = m_BaseSpeed;
	}

	void Paddle::Reset()
	{
		// reset player/ball stats
		GetTransformComponent().Scale = m_BaseScale;
		GetTransformComponent().Translation.x = m_HorizontalBound * 0.5f;
	}

	void Paddle::HandleInput()
	{
		if(Input::IsKeyPressed(Key::KEY_A)) { GetPhysicsComponent().Velocity.x = -m_CurrentSpeed; }
		if(Input::IsKeyPressed(Key::KEY_D)) { GetPhysicsComponent().Velocity.x = m_CurrentSpeed; }
	}

	void Paddle::OnUpdate(Timestep ts)
	{			
		//Reset horizontal speed
		GetPhysicsComponent().Velocity.x = 0; 
		HandleInput();	
		Entity::OnUpdate(ts);
		//Clamp horizontal position to level bounds
		GetTransformComponent().Translation.x = glm::clamp(GetTransformComponent().Translation.x, GetTransformComponent().Scale.x * 0.5f, m_HorizontalBound - GetTransformComponent().Scale.x * 0.5f);
	}
}