#pragma once

#include "DFGEngine.h"
#include "DFGEngine/Renderer/EditorCamera.h"
#include "Game/Level.h"
#include "Game/Paddle.h"
#include "Game/Ball.h"

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
		void OnRender();

		void OnEvent(Event& e) override;
		bool OnWindowResized(WindowResizeEvent& e);

	private:
		void HandleInput();
		void DetectCollisions();
		void RenderSky();

		void ResetLevel();
		void ResetPlayer();

	private:
		Ref<FrameBuffer> m_Framebuffer;
		EditorCamera m_EditorCamera;

		GameState m_GameState = GAME_ACTIVE;
		float m_LevelWitdh = 25;
		float m_LevelHeight = 20;
		std::vector<Level> m_Levels;
		uint32_t m_CurrentLevel = 0;

		Ref<Paddle> m_Paddle;
		Ref<Ball> m_Ball;

	};
}