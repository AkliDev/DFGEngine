#pragma once
#include "Entity.h"
#include "DFGEngine/Renderer/Camera.h"
#include "DFGEngine/Core/Timestep.h"
#include "DFGEngine/Events/Event.h"

#include <glm/glm.hpp>

namespace DFGEngine {

	class GameCamera : public ProjectionCamera
	{
	public:
		GameCamera() = default;
		GameCamera(float fov, float aspectRatio, float nearClip, float farClip, glm::vec3 position =  glm::vec3(0.0f));

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		void SetLookatTarget(const Ref<Entity>& target) { m_LookatTarget = target; }
	private:

		virtual void UpdateView() override;
	private:
		Ref<Entity> m_LookatTarget;
	};
}