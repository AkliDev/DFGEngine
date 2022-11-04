#include "Sandbox2D.h"
#include "Game/Utility.h"

namespace DFGEngine
{
	Sandbox2D::Sandbox2D()
		:Layer("Sandbox2D")
	{

	}
	
	void Sandbox2D::OnAttach()
	{
		//create frame buffer
		FramebufferSpecification fbSpec;
		fbSpec.AttachmentSpecification = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::Depth };
		fbSpec.Width = Application::Get().GetWindow().GetWidth();
		fbSpec.Height = Application::Get().GetWindow().GetHeight();
		m_Framebuffer = FrameBuffer::Create(fbSpec);

		m_EditorCamera = EditorCamera(45.0f, 1.77777777778f, 0.1f, 1000.0f);

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
		Renderer2D::s_TextureLibrary.Load("particle", "assets/textures/particle.png");
		Renderer2D::s_TextureLibrary.Load("powerup_speed", "assets/textures/powerup_speed.png");
		Renderer2D::s_TextureLibrary.Load("powerup_sticky", "assets/textures/powerup_sticky.png");
		Renderer2D::s_TextureLibrary.Load("powerup_increase", "assets/textures/powerup_increase.png");
		Renderer2D::s_TextureLibrary.Load("powerup_confuse", "assets/textures/powerup_confuse.png");
		Renderer2D::s_TextureLibrary.Load("powerup_chaos", "assets/textures/powerup_chaos.png");
		Renderer2D::s_TextureLibrary.Load("powerup_passthrough", "assets/textures/powerup_passthrough.png");

		// Load levels
		Level one; one.Load("assets/levels/one.lvl", m_LevelWitdh, m_LevelHeight * 0.5);
		Level two; two.Load("assets/levels/two.lvl", m_LevelWitdh, m_LevelHeight * 0.5);
		Level three; three.Load("assets/levels/three.lvl", m_LevelWitdh, m_LevelHeight * 0.5);
		Level four; four.Load("assets/levels/four.lvl", m_LevelWitdh, m_LevelHeight * 0.5);
		m_Levels.push_back(one);
		m_Levels.push_back(two);
		m_Levels.push_back(three);
		m_Levels.push_back(four);
		m_CurrentLevel = 0;

		//Create player
		glm::vec3 scale{ 4.0f,0.5f,1.0f };
		glm::vec3 pos{ m_LevelWitdh * 0.5f, scale.y * 0.5f, 0.0f };
		m_Paddle = CreateRef<Paddle>(pos, scale, Renderer2D::s_TextureLibrary.Get("paddle"));
		m_Paddle->Init(m_LevelWitdh, 20);

		//Create ball
		float ballRadius = 0.5f;
		pos = m_Paddle->GetTransformComponent().Translation;
		pos.y += (m_Paddle->GetTransformComponent().Scale.y * 0.5f) + (ballRadius);
		m_Ball = CreateRef<Ball>(pos, ballRadius, Renderer2D::s_TextureLibrary.Get("face"));
		m_Ball->Init(m_Paddle, m_LevelWitdh, m_LevelHeight, 10);
	}

	void Sandbox2D::OnDetach()
	{
		
	}

	void Sandbox2D::HandleInput()
	{
		if (m_GameState == GAME_ACTIVE)
		{
			if (Input::IsKeyPressed(Key::KEY_SPACE))
			{
				m_Ball->Stuck(false);
			}
		}
	}

	void Sandbox2D::DetectCollisions()
	{
		for (Ref<Brick> brick : m_Levels[m_CurrentLevel].GetBricks())
		{
			if (!brick->IsDestroyed())
			{
				Collision collision = CheckCollision(m_Ball, brick);
				if (std::get<0>(collision) == true)
				{
					if (!brick->IsSolid())
					{
						brick->Destroyed(true);
					}
					// collision resolution
					Direction dir = std::get<1>(collision);
					glm::vec2 diff_vector = std::get<2>(collision);
					if (dir == LEFT || dir == RIGHT)
					{
						m_Ball->GetPhysicsComponent().Velocity.x = -m_Ball->GetPhysicsComponent().Velocity.x;
						// relocate
						float penetration = m_Ball->GetRadius() - std::abs(diff_vector.x);
						if (dir == LEFT)
							m_Ball->GetTransformComponent().Translation.x += penetration; 
						else
							m_Ball->GetTransformComponent().Translation.x -= penetration;
					}
					else // vertical collision
					{
						m_Ball->GetPhysicsComponent().Velocity.y = -m_Ball->GetPhysicsComponent().Velocity.y;
						// relocate
						float penetration = m_Ball->GetRadius() - std::abs(diff_vector.y);
						if (dir == UP)
							m_Ball->GetTransformComponent().Translation.y -= penetration; 
						else
							m_Ball->GetTransformComponent().Translation.y += penetration; 
					}
				}
			}
		}

		Collision result = CheckCollision(m_Ball, m_Paddle);
		if (m_Ball->IsStuck() == false && std::get<0>(result))
		{
			// check where it hit the board, and change velocity based on where it hit the board
			float distance = (m_Ball->GetTransformComponent().Translation.x + m_Ball->GetRadius()) - m_Paddle->GetTransformComponent().Translation.x;
			float percentage = distance / (m_Paddle->GetTransformComponent().Scale.x);
			// then move accordingly
			float strength = 2.0f;
			glm::vec2 oldVelocity = m_Ball->GetPhysicsComponent().Velocity;
			m_Ball->GetPhysicsComponent().Velocity.x = m_Ball->m_GetBaseSpeed() * percentage * strength;
			m_Ball->GetPhysicsComponent().Velocity = glm::normalize(m_Ball->GetPhysicsComponent().Velocity) * glm::length(oldVelocity);
			m_Ball->GetPhysicsComponent().Velocity.y = 1.0f * abs(m_Ball->GetPhysicsComponent().Velocity.y);
		}
	}

	void Sandbox2D::ResetLevel()
	{
		m_Levels[m_CurrentLevel].Reset();
	}

	void Sandbox2D::ResetPlayer()
	{
		// reset player/ball stats
		m_Paddle->Reset();
		m_Ball->Stuck(true);
	}

	void Sandbox2D::RenderSky()
	{
		// My engine can only render quads. I am too lazy to design an API for cubemaps. 
		RenderCommand::EnableDepthMask(false);
		Renderer2D::BeginScene(m_EditorCamera);
		Renderer2D::DrawRotatedQuad(m_EditorCamera.GetPosition() + glm::vec3(0.0f, 0.5f, 0.0f), { 1.0f,1.0f }, glm::vec3(glm::radians(90.0f), 0.0f, 0.0f), Renderer2D::s_TextureLibrary.Get("sky_top"));
		Renderer2D::DrawRotatedQuad(m_EditorCamera.GetPosition() + glm::vec3(0.0f, -0.5f, 0.0f), { 1.0f,1.0f }, glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f), Renderer2D::s_TextureLibrary.Get("sky_bottom"));
		Renderer2D::DrawRotatedQuad(m_EditorCamera.GetPosition() + glm::vec3(-0.5f, 0.0f, 0.0f), { 1.0f,1.0f }, glm::vec3(0.0f, glm::radians(90.0f), 0.0f), Renderer2D::s_TextureLibrary.Get("sky_left"));
		Renderer2D::DrawRotatedQuad(m_EditorCamera.GetPosition() + glm::vec3(0.5f, 0.0f, 0.0f), { 1.0f,1.0f }, glm::vec3(0.0f, glm::radians(-90.0f), 0.0f), Renderer2D::s_TextureLibrary.Get("sky_right"));
		Renderer2D::DrawRotatedQuad(m_EditorCamera.GetPosition() + glm::vec3(0.0f, 0.0f, -0.5f), { 1.0f,1.0f }, glm::vec3(0.0f, 0.0f, 0.0f), Renderer2D::s_TextureLibrary.Get("sky_front"));
		Renderer2D::DrawRotatedQuad(m_EditorCamera.GetPosition() + glm::vec3(0.0f, 0.0f, 0.5f), { -1.0f, 1.0f }, glm::vec3(0.0f, 0.0f, 0.0f), Renderer2D::s_TextureLibrary.Get("sky_back"));
		Renderer2D::EndScene();
		RenderCommand::EnableDepthMask(true);
	}

	void Sandbox2D::OnUpdate(Timestep ts)
	{
		HandleInput();

		m_Paddle->OnUpdate(ts);
		m_Ball->OnUpdate(ts);
		m_EditorCamera.OnUpdate(ts);

		DetectCollisions();

		if (m_Ball->GetTransformComponent().Translation.y <0) 
		{
			//this->ResetLevel();
			this->ResetPlayer();
		}

		OnRender();
	}

	void Sandbox2D::OnRender()
	{
		Renderer2D::ResetStats();

		m_Framebuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RenderCommand::Clear();

		RenderSky();

		Renderer2D::BeginScene(m_EditorCamera);	
		m_Levels[m_CurrentLevel].OnRender();
		m_Paddle->OnRender();
		m_Ball->OnRender();	
		Renderer2D::EndScene();

		m_Framebuffer->Unbind();

		RenderCommand::SetClearColor({ 0.1, 0.1, 0.1, 1 });
		RenderCommand::Clear();

		Renderer2D::BeginScene(glm::mat4(1.0f));
		Renderer2D::DrawFrame(m_Framebuffer->GetColorAttachmentAsTexture());
		Renderer2D::EndScene();

		//auto stats = Renderer2D::GetStats();
		//DFG_TRACE(stats.DrawCalls);
	}

	void Sandbox2D::OnEvent(Event& e)
	{
		m_EditorCamera.OnEvent(e);
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(DFG_BIND_EVENT_FN(Sandbox2D::OnWindowResized));
	}

	bool Sandbox2D::OnWindowResized(WindowResizeEvent& e)
	{
		m_Framebuffer->Resize(e.GetWidth(), e.GetHeight());
		m_EditorCamera.SetViewportSize(e.GetWidth(), e.GetHeight());
		return false;
	}
}