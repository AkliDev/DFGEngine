#pragma once
#include "DFGEngine.h"
#include "Entity.h"

namespace DFGEngine
{
	class Paddle : public Entity
	{
	public:
		Paddle() = default;
		Paddle(glm::vec3 pos, glm::vec3 scale, Ref<Texture2D> sprite, glm::vec4 color = glm::vec4(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));
	
		void OnUpdate(Timestep ts) override;
		void Init(float horizonatalBound, float speed);
		void Reset();

	private:
		void HandleInput();

	private:
		glm::vec3 m_BaseScale;
		float m_HorizontalBound = 20;
		float m_BaseSpeed = 20;
		float m_CurrentSpeed = m_BaseSpeed;
	};
}