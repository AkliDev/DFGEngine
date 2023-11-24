#include "Entity.h"

namespace DFGEngine
{
	Entity::Entity(glm::vec3 pos, glm::vec3 scale, Ref<Texture2D> texture, glm::vec4 color, glm::vec2 velocity)
	{
		m_TransformComponent = TransformComponent();
		m_TransformComponent.Translation = pos;
		m_TransformComponent.Scale = scale;

		m_SpiteRendererComponent = SpriteRendererComponent();
		m_SpiteRendererComponent.Texture = texture;
		m_SpiteRendererComponent.Color = color;

		m_PhysicsComponent = PhysicsComponent();
		m_PhysicsComponent.Velocity = velocity;
	}

	void Entity::OnUpdate(Timestep ts)
	{
		//Apply Velocity to position
		GetTransformComponent().Translation.x += GetPhysicsComponent().Velocity.x * ts;
		GetTransformComponent().Translation.y += GetPhysicsComponent().Velocity.y * ts;
		////Apply AngularVelocity to Rotation
		GetTransformComponent().Rotation.z += GetPhysicsComponent().AngularVelocity * ts;
	}

	void Entity::OnRender()
	{
		Renderer2D::DrawQuad({ GetTransform() }, GetSpiteRendererComponent().Texture);
	}
}