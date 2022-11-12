#pragma once

#include "Camera.h"
#include "DFGEngine/Core/Timestep.h"
#include "DFGEngine/Events/Event.h"
#include "DFGEngine/Events/MouseEvent.h"

#include <glm/glm.hpp>

namespace DFGEngine {

	class EditorCamera : public ProjectionCamera
	{
	public:
		EditorCamera() = default;
		EditorCamera(float fov, float aspectRatio, float nearClip, float farClip, glm::vec3 position =  glm::vec3(0.0f));

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		float GetDistance() const { return m_Distance; }
		void SetDistance(float distance) { m_Distance = distance; }

	private:
		virtual void UpdateView() override;

		bool OnMouseScroll(MouseScrolledEvent& e);

		void MousePan(const glm::vec2& delta);
		void MouseRotate(const glm::vec2& delta);
		void MouseZoom(float delta);

		glm::vec3 CalculatePosition() const;

		std::pair<float, float> PanSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;
	private:
		glm::vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };
		glm::vec2 m_InitialMousePosition = { 0.0f, 0.0f };

		float m_Distance = 10.0f;
	};
}