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

	class ProjectionCamera : public Camera
	{
	public:
		ProjectionCamera() = default;
		ProjectionCamera(float fov, float aspectRatio, float nearClip, float farClip, glm::vec3 position = glm::vec3(0.0f));

		inline void SetViewportSize(float width, float height) { m_ViewportWidth = width; m_ViewportHeight = height; UpdateProjection(); }

		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		glm::mat4 GetViewProjection() const { return m_Projection * m_ViewMatrix; }

		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwardDirection() const;
		const glm::vec3& GetPosition() const { return m_Position; }
		glm::quat GetOrientation() const;

		float GetRoll() const { return m_Rotation.z; }
		float GetPitch() const { return m_Rotation.x; }
		float GetYaw() const { return m_Rotation.y; }

		void SetFOV(float fov) { m_FOV = glm::clamp(fov, 30.0f, 120.0f); UpdateProjection(); }
		void SetPosition(glm::vec3 position) { m_Position = position; }
		void SetRotation(glm::vec3 rotation) { m_Rotation = rotation; }
	protected:
		void UpdateProjection();
		virtual void UpdateView();
	protected:
		float m_FOV = 45.0f, m_AspectRatio = 1.778f, m_NearClip = 0.1f, m_FarClip = 1000.0f;
		float m_ViewportWidth = 1280, m_ViewportHeight = 720;

		glm::mat4 m_ViewMatrix;
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };	
		glm::vec3 m_Rotation = { 0.0f, 0.0f, 0.0f };
	};
}