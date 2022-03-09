#pragma once

#include "DFGEngine.h"
#include "DFGEngine/Renderer/EditorCamera.h"

namespace DFGEngine
{
	class Sandbox2D : public Layer
	{
	public:
		Sandbox2D();
		virtual ~Sandbox2D() = default;

		void OnAttach() override;
		void OnDetach() override;

		void OnUpdate() override;

		void OnEvent(Event& e) override;
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		Ref<FrameBuffer> m_Framebuffer;

		Ref<Scene> m_ActiveScene;
		EditorCamera m_EditorCamera;
	};
}