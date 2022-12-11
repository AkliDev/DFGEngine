#include "Brick.h"

namespace DFGEngine
{
	Brick::Brick(glm::vec3 pos, glm::vec3 scale, Ref<Texture2D> sprite, glm::vec4 color, glm::vec2 velocity)
		:Entity(pos, scale, sprite, color, velocity) 
	{
		glm::mat4 transform = GetTransformComponent().GetTransform();
		m_Transforms[0] = glm::translate(transform, { 0.0f,-0.5f,0.0f }) * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), { 1.0f, 0.0f, 0.0f });	//Bottom
		m_Transforms[1] = glm::translate(transform, { -0.5f,0.0f,0.0f }) * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), { 0.0f, 1.0f, 0.0f }); //left
		m_Transforms[2] = glm::translate(transform, {0.0f,0.0f,0.5f});		//front
		m_Transforms[3] = glm::translate(transform, { 0.5f,0.0f,0.0f }) * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), { 0.0f, 1.0f, 0.0f });	//right
		m_Transforms[4] = glm::translate(transform, { 0.0f,0.5f,0.0f }) * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), { 1.0f, 0.0f, 0.0f });	//Top
	}

	void Brick::OnUpdate(Timestep ts)
	{

	}

	void Brick::OnRender()
	{
		for (int i = 0; i < 5; i++)
		{
			Renderer2D::DrawQuad(m_Transforms[i], GetSpiteRendererComponent().Texture, GetSpiteRendererComponent().TilingFactor, GetSpiteRendererComponent().Color);
		}
	}
}