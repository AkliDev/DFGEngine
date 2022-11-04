#include "dfgpch.h"
#include "Scene.h"

//#include "Components.h"
#include "DFGEngine/Renderer/Renderer2D.h"

namespace DFGEngine
{
	Scene::Scene()
	{

	}

	Scene::~Scene()
	{

	}

	void Scene::OnUpdate(Timestep ts)
	{
		
	}

	void Scene::OnRender(EditorCamera Camera)
	{

	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;
	}
}
