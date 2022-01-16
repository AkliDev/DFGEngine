#include "Sandbox2D.h"
#include <glad/glad.h>

namespace DFGEngine
{
	Sandbox2D::Sandbox2D()
		:Layer("Sandbox2D")
	{

	}

	void Sandbox2D::OnAttach()
	{
		FramebufferSpecification fbSpec;
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::Depth };
		fbSpec.Width = Application::Get().GetWindow().GetWidth();
		fbSpec.Height = Application::Get().GetWindow().GetHeight();
		m_Framebuffer = FrameBuffer::Create(fbSpec);

		m_Texture = Texture2D::Create("assets/textures/Goombah.png");;
	}

	void Sandbox2D::OnDetach()
	{

	}

	void Sandbox2D::OnUpdate()
	{
		m_Framebuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RenderCommand::Clear();
		
		Renderer2D::BeginScene(glm::mat4(1.0f));
		Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 2.0f, 2.0f }, m_Texture);
		Renderer2D::EndScene();
		m_Framebuffer->Unbind();

		RenderCommand::SetClearColor({ 0.1, 0.1, 0.1, 1 });
		RenderCommand::Clear();

		Renderer2D::BeginScene(glm::mat4(1.0f));
		Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 2.0f, 2.0f }, m_Framebuffer->GetColorAttachmentAsTexture());
		Renderer2D::EndScene();
	}

	void Sandbox2D::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(DFG_BIND_EVENT_FN(Sandbox2D::OnWindowResized));
	}

	bool Sandbox2D::OnWindowResized(WindowResizeEvent& e)
	{
		m_Framebuffer->Resize(e.GetWidth(), e.GetHeight());
		return false;
	}
}