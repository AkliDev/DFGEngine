#include "Sandbox2D.h"

namespace DFGEngine
{
	Sandbox2D::Sandbox2D()
		:Layer("Sandbox2D")
	{

	}
	
	void Sandbox2D::OnAttach()
	{
		m_ViewportWidth = Application::Get().GetWindow().GetWidth();
		m_ViewportHeight = Application::Get().GetWindow().GetHeight();

		//create frame buffer
		FramebufferSpecification fbSpec;
		fbSpec.AttachmentSpecification = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::Depth };
		fbSpec.Width = m_ViewportWidth;
		fbSpec.Height = m_ViewportHeight;
		m_Framebuffer = FrameBuffer::Create(fbSpec);

		m_EditorCamera = EditorCamera(45.0f, 1.77777777778f, 0.1f, 1000.0f);

		//load audio
		SoundEngine::LoadSFX("protector","assets/audio/sfx/iamtheprotectorofthissystem.wav");
		SoundEngine::LoadSFX("solid", "assets/audio/sfx/solid.wav");
		SoundEngine::LoadSFX("bloop", "assets/audio/sfx/bloop.wav");
		SoundEngine::LoadSFX("bleep", "assets/audio/sfx/bleep.wav");
		SoundEngine::LoadSFX("powerup", "assets/audio/sfx/powerup.wav");

		SoundEngine::LoadMusic("BGM1", "assets/audio/bgm/Juhani Junkala [Retro Game Music Pack] Title Screen.wav");
		SoundEngine::LoadMusic("BGM2", "assets/audio/bgm/Juhani Junkala [Retro Game Music Pack] Level 1.wav");
		SoundEngine::LoadMusic("BGM3", "assets/audio/bgm/Juhani Junkala [Retro Game Music Pack] Level 2.wav");
		SoundEngine::LoadMusic("BGM4", "assets/audio/bgm/Juhani Junkala [Retro Game Music Pack] Level 3.wav");
		SoundEngine::LoadMusic("BGM5", "assets/audio/bgm/Juhani Junkala [Retro Game Music Pack] Ending.wav");

		//load textures
		Renderer2D::s_TextureLibrary.Load("sky_top", "assets/textures/lightblue/top.png");
		Renderer2D::s_TextureLibrary.Load("sky_bottom", "assets/textures/lightblue/bottom.png");
		Renderer2D::s_TextureLibrary.Load("sky_left", "assets/textures/lightblue/left.png");
		Renderer2D::s_TextureLibrary.Load("sky_right", "assets/textures/lightblue/right.png");
		Renderer2D::s_TextureLibrary.Load("sky_front", "assets/textures/lightblue/front.png");
		Renderer2D::s_TextureLibrary.Load("sky_back", "assets/textures/lightblue/back.png");
		
		Renderer2D::s_TextureLibrary.Load("face", "assets/textures/Goombah.png");
		Renderer2D::s_TextureLibrary.Load("block", "assets/textures/block.png");
		Renderer2D::s_TextureLibrary.Load("block_solid", "assets/textures/block_solid.png");
		Renderer2D::s_TextureLibrary.Load("paddle", "assets/textures/paddle.png");
		//Renderer2D::s_TextureLibrary.Load("particle", "assets/textures/particle.png");

		m_Game.Init();
	}

	void Sandbox2D::OnDetach()
	{
		
	}

	void Sandbox2D::OnUpdate(Timestep ts)
	{
		m_Game.OnUpdate();
		m_EditorCamera.OnUpdate(ts);
		OnRender(m_EditorCamera);
	}

	void Sandbox2D::RenderSky(ProjectionCamera& camera)
	{
		////I'll do proper cubemaps later.
		//float boxScale = 10.0f;
		//RenderCommand::EnableDepthMask(false);
		//Renderer2D::BeginScene(camera);
		//Renderer2D::DrawRotatedQuad(camera.GetPosition() + glm::vec3(0.0f, boxScale * 0.5f, 0.0f), glm::vec2(boxScale), glm::vec3(glm::radians(90.0f), 0.0f, 0.0f), Renderer2D::s_TextureLibrary.Get("sky_top"));
		//Renderer2D::DrawRotatedQuad(camera.GetPosition() + glm::vec3(0.0f, boxScale * -0.5f, 0.0f), glm::vec2(boxScale), glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f), Renderer2D::s_TextureLibrary.Get("sky_bottom"));
		//Renderer2D::DrawRotatedQuad(camera.GetPosition() + glm::vec3(boxScale * -0.5f, 0.0f, 0.0f), glm::vec2(boxScale), glm::vec3(0.0f, glm::radians(90.0f), 0.0f), Renderer2D::s_TextureLibrary.Get("sky_left"));
		//Renderer2D::DrawRotatedQuad(camera.GetPosition() + glm::vec3(boxScale * 0.5f, 0.0f, 0.0f), glm::vec2(boxScale), glm::vec3(0.0f, glm::radians(-90.0f), 0.0f), Renderer2D::s_TextureLibrary.Get("sky_right"));
		//Renderer2D::DrawRotatedQuad(camera.GetPosition() + glm::vec3(0.0f, 0.0f, boxScale * -0.5f), glm::vec2(boxScale), glm::vec3(0.0f, 0.0f, 0.0f), Renderer2D::s_TextureLibrary.Get("sky_front"));
		//Renderer2D::DrawRotatedQuad(camera.GetPosition() + glm::vec3(0.0f, 0.0f, boxScale * 0.5f), glm::vec2(boxScale), glm::vec3(0.0f, glm::radians(-180.0f), 0.0f), Renderer2D::s_TextureLibrary.Get("sky_back"));
		//Renderer2D::EndScene();
		//RenderCommand::EnableDepthMask(true);
	}

	void Sandbox2D::OnRender(ProjectionCamera& camera)
	{
		Renderer2D::ResetStats();

		m_Framebuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RenderCommand::Clear();
		RenderSky(camera);
		Renderer2D::BeginScene(camera);
		m_Game.OnRender();
		if (Input::IsKeyPressed(Key::KEY_L)) { LuaBinding(); }
		Renderer2D::EndScene();	
		m_Framebuffer->Unbind();

		RenderCommand::SetClearColor({ 0.1, 0.1, 0.1, 1 });
		RenderCommand::Clear();
		Renderer2D::BeginScene(glm::mat4(1.0f));
		Renderer2D::DrawFrame(m_Framebuffer->GetColorAttachmentAsTexture());
		Renderer2D::EndScene();

		glm::mat4 projection = glm::ortho(0.0f, m_ViewportWidth, 0.0f, m_ViewportHeight);
		Renderer2D::BeginScene(projection);
		m_Game.OnUIRender();
		Renderer2D::EndScene();
	
		//auto stats = Renderer2D::GetStats();
		//DFG_TRACE(stats.DrawCalls);
	}

	void Sandbox2D::OnEvent(Event& e)
	{
		m_EditorCamera.OnEvent(e);
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(DFG_BIND_EVENT_FN(Sandbox2D::OnKeyPressed));
		dispatcher.Dispatch<WindowResizeEvent>(DFG_BIND_EVENT_FN(Sandbox2D::OnWindowResized));
	}

	bool Sandbox2D::OnKeyPressed(KeyPressedEvent& e)
	{
		if (e.IsRepeat() == true)
		{
			return false;
		}

		return false;
	}

	bool Sandbox2D::OnWindowResized(WindowResizeEvent& e)
	{
		m_ViewportWidth = e.GetWidth();
		m_ViewportHeight = e.GetHeight();

		m_Framebuffer->Resize(e.GetWidth(), e.GetHeight());
		m_EditorCamera.SetViewportSize(e.GetWidth(), e.GetHeight());
		return false;
	}
}