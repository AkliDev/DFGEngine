#pragma once

#include "DFGEngine/Core/Timestep.h"
#include "DFGEngine/Renderer/EditorCamera.h"

namespace DFGEngine
{	
	class Scene
	{
	public:
		Scene();
		~Scene();

		void OnUpdate(Timestep ts);
		void OnRender(EditorCamera Camera);
		void OnViewportResize(uint32_t width, uint32_t height);

	private:
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
	};
}
