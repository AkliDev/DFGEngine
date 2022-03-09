#pragma once

//#include "DFGEngine/Core/Timestep.h"
#include "DFGEngine/Renderer/EditorCamera.h"

namespace DFGEngine
{	
	class Entity;
	class Scene
	{
	public:
		Scene();
		~Scene();

		void OnUpdate(/*Timestep ts*/);
		void OnRender(/*Timestep ts*/ EditorCamera Camera);
		void OnViewportResize(uint32_t width, uint32_t height);

	private:
		//entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
	};
}
