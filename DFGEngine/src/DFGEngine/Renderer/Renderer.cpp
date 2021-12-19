#include "dfgpch.h"
#include "Renderer.h"

//#include "Alternate/Renderer/Renderer2D.h"

//#include "Platform/OpenGL/OpenGLShader.h"

namespace DFGEngine
{
	//Scope<Renderer::SceneData> Renderer::s_SceneData = CreateScope<Renderer::SceneData>();

	void Renderer::Init()
	{
		//DFG_PROFILE_FUNCTION();

		RenderCommand::Init();
		//Renderer2D::Init();
	}

	void Renderer::Shutdown()
	{
		//Renderer2D::ShutDown();
	}

	void Renderer::OnWindowResized(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	//void Renderer::BeginScene(OrthographicCamera& camera)
	//{
	//	s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	//}

	//void Renderer::EndScene()
	//{
	//}

	//void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	//{
	//	shader->Bind();
	//	std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
	//	std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);
	//
	//	vertexArray->Bind();
	//	RenderCommand::DrawIndexed(vertexArray);
	//}
}
