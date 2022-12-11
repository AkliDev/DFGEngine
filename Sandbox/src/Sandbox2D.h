#pragma once

#include "DFGEngine.h"

#include "DFGEngine/Renderer/EditorCamera.h"

#include "Game/GameCamera.h"
#include "Game/Level.h"
#include "Game/Paddle.h"
#include "Game/Ball.h"

#include "LuaBinding.h"
#include "TestRegistration.h"

namespace DFGEngine
{
	class Sandbox2D : public Layer
	{
		// Represents the current state of the game
		enum GameState
		{
			GAME_ACTIVE,
			GAME_MENU,
			GAME_WIN
		};

	public:
		Sandbox2D();
		virtual ~Sandbox2D() = default;

		void OnAttach() override;
		void OnDetach() override;

		void OnUpdate(Timestep ts) override;
	
		void OnEvent(Event& e) override;
		bool OnWindowResized(WindowResizeEvent& e);

	private:
		bool OnKeyPressed(KeyPressedEvent& e);
		void OnRender(ProjectionCamera& camera);
		void RenderSky(ProjectionCamera& camera);
		void DetectCollisions();
		
		void ResetLevel();
		void ResetPlayer();

	private:
		float m_ViewportWidth, m_ViewportHeight;
		Ref<FrameBuffer> m_Framebuffer;
		EditorCamera m_EditorCamera;
		GameCamera m_GameCamera;
	
		GameState m_GameState = GAME_MENU;
		float m_LevelWitdh = 25;
		float m_LevelHeight = 20;
		std::vector<Level> m_Levels;
		uint32_t m_CurrentLevel = 0;

		Ref<Entity> m_Tracker;
		Ref<Paddle> m_Paddle;
		Ref<Ball> m_Ball;
		uint32_t m_Lives = 2;
	};
}