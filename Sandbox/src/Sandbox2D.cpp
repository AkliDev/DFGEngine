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
		m_ViewportWidth = Application::Get().GetWindow().GetWidth();
		m_ViewportHeight = Application::Get().GetWindow().GetHeight();

		//create frame buffer
		FramebufferSpecification fbSpec;
		fbSpec.AttachmentSpecification = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::Depth };
		fbSpec.Width = m_ViewportWidth;
		fbSpec.Height = m_ViewportHeight;
		m_Framebuffer = FrameBuffer::Create(fbSpec);

		m_EditorCamera = EditorCamera(45.0f, 1.77777777778f, 0.1f, 1000.0f);
		m_GameCamera = GameCamera(90.0f, 1.77777777778f, 0.1f, 1000.0f);

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
		m_Ball->Init(m_Paddle, m_LevelWitdh, m_LevelHeight, 15);

		m_Tracker = CreateRef<Entity>(glm::vec3(m_LevelWitdh * 0.5f,-2 + m_LevelHeight * 0.5f + 2.0f, 0.0f));
		m_GameCamera.SetLookatTarget(m_Tracker);
		SoundEngine::PlayMusic("BGM5");
		SoundEngine::PlaySFX("protector");
	}

	void Sandbox2D::OnDetach()
	{
		
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
						SoundEngine::PlaySFX("bleep");
					}
					else
					{
						SoundEngine::PlaySFX("solid");
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
		if (m_Ball->IsStuck() == false && std::get<0>(result) && m_Ball->GetPhysicsComponent().Velocity.y < 0)
		{
			// check where it hit the board, and change velocity based on where it hit the board
			float distance = (m_Ball->GetTransformComponent().Translation.x + m_Ball->GetRadius()) - m_Paddle->GetTransformComponent().Translation.x;
			float percentage = distance / (m_Paddle->GetTransformComponent().Scale.x);
			// then move accordingly
			float strength = 3.0f;
			glm::vec2 oldVelocity = m_Ball->GetPhysicsComponent().Velocity;
			m_Ball->GetPhysicsComponent().Velocity.x = m_Ball->m_GetBaseSpeed() * percentage * strength;
			m_Ball->GetPhysicsComponent().Velocity = glm::normalize(m_Ball->GetPhysicsComponent().Velocity) * glm::length(oldVelocity);
			m_Ball->GetPhysicsComponent().Velocity.y = 1.0f * abs(m_Ball->GetPhysicsComponent().Velocity.y);
			SoundEngine::PlaySFX("bloop");
		}
	}

	void Sandbox2D::ResetLevel()
	{
		m_Lives = 3;
		m_GameCamera.SetLookatTarget(m_Tracker);
		m_GameCamera.SetFOV(90.0f);
		m_Levels[m_CurrentLevel].Reset();
		SoundEngine::PlayMusic("BGM5");
	}

	void Sandbox2D::ResetPlayer()
	{
		// reset player/ball stats
		m_Paddle->Reset();
		m_Ball->Stuck(true);
	}

	void Sandbox2D::OnUpdate(Timestep ts)
	{		
		if (m_GameState == GAME_ACTIVE)
		{
			// check loss condition
			if (m_Ball->GetTransformComponent().Translation.y < -4)
			{

				if (m_Lives == 0)
				{
					ResetLevel();
					m_GameState = GAME_MENU;
				}
				ResetPlayer();
				m_Lives--;
				SoundEngine::PlaySFX("powerup");
			}
			// check win condition
			if (m_Levels[m_CurrentLevel].IsCompleted())
			{
				ResetLevel();
				ResetPlayer();
				m_GameState = GAME_WIN;
				SoundEngine::PlaySFX("powerup");
			}

			m_Paddle->OnUpdate(ts);
			m_Ball->OnUpdate(ts);
			DetectCollisions();
		}
		else if (m_GameState == GAME_MENU || m_GameState == GAME_WIN)
		{
			float radius = 10;
			static float value = 0;	
			value += 0.4f * ts;
			m_GameCamera.SetPosition(m_LevelWitdh * 0.5f + glm::vec3(sin(value) * radius,-5 + m_LevelHeight * 0.5f + cos(value) * radius, 0.0f));
		}

		m_EditorCamera.OnUpdate(ts);
		m_GameCamera.OnUpdate(ts);
		OnRender(m_GameCamera);
	}

	void Sandbox2D::RenderSky(ProjectionCamera& camera)
	{
		//I'll do proper cubemaps later.
		float boxScale = 10.0f;
		RenderCommand::EnableDepthMask(false);
		Renderer2D::BeginScene(camera);
		Renderer2D::DrawRotatedQuad(camera.GetPosition() + glm::vec3(0.0f, boxScale * 0.5f, 0.0f), glm::vec2(boxScale), glm::vec3(glm::radians(90.0f), 0.0f, 0.0f), Renderer2D::s_TextureLibrary.Get("sky_top"));
		Renderer2D::DrawRotatedQuad(camera.GetPosition() + glm::vec3(0.0f, boxScale * -0.5f, 0.0f), glm::vec2(boxScale), glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f), Renderer2D::s_TextureLibrary.Get("sky_bottom"));
		Renderer2D::DrawRotatedQuad(camera.GetPosition() + glm::vec3(boxScale * -0.5f, 0.0f, 0.0f), glm::vec2(boxScale), glm::vec3(0.0f, glm::radians(90.0f), 0.0f), Renderer2D::s_TextureLibrary.Get("sky_left"));
		Renderer2D::DrawRotatedQuad(camera.GetPosition() + glm::vec3(boxScale * 0.5f, 0.0f, 0.0f), glm::vec2(boxScale), glm::vec3(0.0f, glm::radians(-90.0f), 0.0f), Renderer2D::s_TextureLibrary.Get("sky_right"));
		Renderer2D::DrawRotatedQuad(camera.GetPosition() + glm::vec3(0.0f, 0.0f, boxScale * -0.5f), glm::vec2(boxScale), glm::vec3(0.0f, 0.0f, 0.0f), Renderer2D::s_TextureLibrary.Get("sky_front"));
		Renderer2D::DrawRotatedQuad(camera.GetPosition() + glm::vec3(0.0f, 0.0f, boxScale * 0.5f), glm::vec2(boxScale), glm::vec3(0.0f, glm::radians(-180.0f), 0.0f), Renderer2D::s_TextureLibrary.Get("sky_back"));
		Renderer2D::EndScene();
		RenderCommand::EnableDepthMask(true);
	}

	void Sandbox2D::OnRender(ProjectionCamera& camera)
	{
		Renderer2D::ResetStats();

		m_Framebuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RenderCommand::Clear();
		RenderSky(camera);
		Renderer2D::BeginScene(camera);
		m_Levels[m_CurrentLevel].OnRender();
		m_Paddle->OnRender();
		m_Ball->OnRender();
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
		
		if (m_GameState == GAME_WIN)
		{
			Renderer2D::s_TextRenderer.DrawText("You won!", glm::vec3(m_ViewportWidth * 0.5f - 300, m_ViewportHeight * 0.5f + 35, 0.1f), 1.0f);
			Renderer2D::s_TextRenderer.DrawText("Press ENTER to retry or ESC to quit", glm::vec3(m_ViewportWidth * 0.5f - 90, m_ViewportHeight * 0.5f, 0.1f), 0.7f);
		}
		if (m_GameState == GAME_MENU)
		{
			Renderer2D::s_TextRenderer.DrawText("Press ENTER to start", glm::vec3(m_ViewportWidth * 0.5f - 300, m_ViewportHeight * 0.5f + 35, 0.1f), 1.0f);
			Renderer2D::s_TextRenderer.DrawText("Press A or D to select level", glm::vec3(m_ViewportWidth * 0.5f - 90, m_ViewportHeight * 0.5f, 0.1f), 0.7f);
		}
		if (m_GameState == GAME_ACTIVE)
		{
			std::stringstream ss;
			ss << "Lives: " << m_Lives;
			Renderer2D::s_TextRenderer.DrawText(ss.str(), glm::vec3(0, m_ViewportHeight - 40.0f, 0.1f), 1.0f);
		}
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

		if (m_GameState == GAME_ACTIVE)
		{
			if (e.GetKeyCode() == Key::KEY_SPACE) m_Ball->Stuck(false);
		}

		if (m_GameState == GAME_MENU)
		{
			if (e.GetKeyCode() == Key::KEY_RETURN)
			{
				m_GameCamera.SetFOV(47.0f);
				m_GameCamera.SetPosition(glm::vec3(m_LevelWitdh * 0.5f, -3, 17.0f));
				m_GameCamera.SetRotation(glm::vec3(0.57f, 0.0f, 0.0f));
				m_GameCamera.SetLookatTarget(nullptr);

				switch (m_CurrentLevel)
				{
				case 0: SoundEngine::PlayMusic("BGM1"); break;
				case 1: SoundEngine::PlayMusic("BGM2"); break;
				case 2: SoundEngine::PlayMusic("BGM3"); break;
				case 3: SoundEngine::PlayMusic("BGM4"); break;
				}
				
				m_GameState = GAME_ACTIVE;
			}
			if (e.GetKeyCode() == Key::KEY_D) m_CurrentLevel = (m_CurrentLevel + 1) % 4;;
			if (e.GetKeyCode() == Key::KEY_A) m_CurrentLevel = (m_CurrentLevel - 1) % 4;;
		}

		if (m_GameState == GAME_WIN)
		{		
			if (e.GetKeyCode() == Key::KEY_RETURN)
			{
				m_GameState = GAME_MENU;
			}
		}	
		return false;
	}

	bool Sandbox2D::OnWindowResized(WindowResizeEvent& e)
	{
		m_ViewportWidth = e.GetWidth();
		m_ViewportHeight = e.GetHeight();

		m_Framebuffer->Resize(e.GetWidth(), e.GetHeight());
		m_EditorCamera.SetViewportSize(e.GetWidth(), e.GetHeight());
		m_GameCamera.SetViewportSize(e.GetWidth(), e.GetHeight());
		return false;
	}
}