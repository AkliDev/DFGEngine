#pragma once

#include <glm/glm.hpp>

namespace DFGEngine
{
	class Camera
	{
	public:
		Camera() = default;
		Camera(const glm::mat4 projection)
			: m_Projection(projection) {}

		virtual ~Camera() = default;

		const glm::mat4 GetProjection() const { return m_Projection; }
	protected:
		glm::mat4 m_Projection = glm::mat4(1.0f);
	};

	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		void SetProjection(float left, float right, float bottom, float top);

		const glm::vec3 GetPosition() const { return m_Postion; }
		void SetPosition(const glm::vec3 postion) { m_Postion = postion; RecalcualteViewMatrix(); }

		const float GetRotation() const { return m_Rotation; }
		void SetRotation(const float rotation) { m_Rotation = rotation; RecalcualteViewMatrix(); }

		const glm::mat4 GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4 GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4 GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	private:
		void RecalcualteViewMatrix();

	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Postion;
		float m_Rotation;
	};
}
