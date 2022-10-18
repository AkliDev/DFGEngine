#include "dfgpch.h"
#include "Scene.h"

//#include "Components.h"
#include "DFGEngine/Renderer/Renderer2D.h"

//#include "Entity.h"
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
		Renderer2D::BeginScene(Camera);
		Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 2.0f, 2.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
		Renderer2D::EndScene();
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;
	}
}
