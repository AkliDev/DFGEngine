#pragma once
#include "DFGEngine.h"
#include "Entity.h"

namespace DFGEngine
{
	class Brick : public Entity
	{
	public:
		Brick() = default;
		Brick(glm::vec3 pos, glm::vec3 scale, Ref<Texture2D> sprite, glm::vec4 color = glm::vec4(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));
		void OnUpdate(Timestep ts) override;
		void OnRender() override;

		void Solid(bool solid) { m_Solid = solid; }
		void Destroyed(bool destroyed) { m_Destroyed = destroyed; }

		bool IsSolid() const { return m_Solid; }
		bool IsDestroyed() const { return m_Destroyed; }

	private:
		bool   m_Solid = false;
		bool   m_Destroyed = false;
		glm::mat4 m_Transforms[6];
	};
}