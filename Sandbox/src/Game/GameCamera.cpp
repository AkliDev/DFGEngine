#include "dfgpch.h"
#include "GameCamera.h"

#include "DFGEngine/Core/Input.h"
#include "DFGEngine/Core/KeyCodes.h"
#include "DFGEngine/Core/MouseCodes.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace DFGEngine 
{
	GameCamera::GameCamera(float fov, float aspectRatio, float nearClip, float farClip, glm::vec3 position)
		: ProjectionCamera(fov, aspectRatio, nearClip, farClip, position)
	{
		UpdateView();
	}

	void GameCamera::OnUpdate(Timestep ts)
	{
		UpdateView();
	}

	void GameCamera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		//dispatcher.Dispatch<MouseScrolledEvent>(DFG_BIND_EVENT_FN(GameCamera::OnMouseScroll));
	}

	void GameCamera::UpdateView()
	{
		glm::quat orientation = GetOrientation();	
		if (m_LookatTarget)
		{
			m_ViewMatrix = glm::lookAt(m_Position, m_LookatTarget->GetTransformComponent().Translation, glm::vec3(0.0, 1.0, 0.0));
		}
		else
		{
			m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);
			m_ViewMatrix = glm::inverse(m_ViewMatrix);
		}
	}
}