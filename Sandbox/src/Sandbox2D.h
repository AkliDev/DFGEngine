#pragma once



#include "DFGEngine/Renderer/EditorCamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Game.h"

#include "LuaBinding.h"
#include "TestRegistration.h"

namespace DFGEngine
{
	class Sandbox2D : public Layer
	{
	public:
		Sandbox2D();
		virtual ~Sandbox2D() = default;

		void OnAttach() override;
		void OnDetach() override;

		void OnUpdate(Timestep ts) override;
	
		void OnEvent(Event& e) override;
		bool OnWindowResized(WindowResizeEvent& e);

	private:
		bool OnKeyPressed(KeyPressedEvent& e);
		void OnRender(ProjectionCamera& camera);
		void RenderSky(ProjectionCamera& camera);

	private:
		float m_ViewportWidth, m_ViewportHeight;
		Ref<FrameBuffer> m_Framebuffer;
		EditorCamera m_EditorCamera;

		Game m_Game;
	};
}