#pragma once
#include "Game.h"

namespace DFGEngine
{
	void Game::Init()
	{
		m_Gamestate.Inputs[0] = (uint32_t)InputCommand::None;
		m_Gamestate.Inputs[1] = (uint32_t)InputCommand::None;
		m_Gamestate.Entities[0].Position.x = -300;
		m_Gamestate.Entities[1].Position.x = 300;

		ApplicationCommandLineArgs args = Application::Get().GetSpecification().CommandLineArgs;	
		if (args.Count >= 2)
		{
			if (args[1][0] == *"s") m_BootNetworkSetting = BootNetworkSetting::Host;
			if (args[1][0] == *"c") m_BootNetworkSetting = BootNetworkSetting::Client;

			//Override rollback mode and run the network in deterministic lock step mode (delay based)
			if (args.Count >= 3)
			{
				if (args[2][0] == *"d") m_NetworkSyncType = NetworkSyncType::LockStep;
			}
		}

		if (m_BootNetworkSetting == BootNetworkSetting::Host)
		{
			m_LocalSide = 0;
			m_OpponentSide = 1;
			m_NetworkState = NetworkState::Host;
			InitializeHost();
		}
		if (m_BootNetworkSetting == BootNetworkSetting::Client)
		{
			m_LocalSide = 1;
			m_OpponentSide = 0;
			m_NetworkState = NetworkState::Client;
			InitializeClient();
		}

		memcpy(&m_SavedGameState, &m_Gamestate, sizeof(SimulatonState));
	}

	void Game::OnUpdate()
	{
		m_Gamestate.Inputs[0] = (uint32_t)InputCommand::None;
		m_Gamestate.Inputs[1] = (uint32_t)InputCommand::None;

		if (m_BootNetworkSetting == BootNetworkSetting::NoConnection)
		{
			//Save Game State
			if (Input::IsKeyPressed(Key::KEY_F5))
			{
				DFG_TRACE("Saved State");
				memcpy(&m_SavedGameState, &m_Gamestate, sizeof(SimulatonState));
			}
			//Load Game State
			else if (Input::IsKeyPressed(Key::KEY_F6))
			{
				DFG_TRACE("Loaded State");
				memcpy(&m_Gamestate, &m_SavedGameState, sizeof(SimulatonState));
			}
		}

		m_NetInputBuffer = (uint32_t)InputCommand::None;

		if (m_UpdatePullInput == true)
		{
			HandleInput();

			//Record this input in the history buffer for sending the the other player.
			if (m_Gamestate.FrameCount < INPUT_HISTORY_SIZE - NETWORK_INPUT_DELAY)
			{
				m_InputHistory[m_LocalSide][m_Gamestate.FrameCount + NETWORK_INPUT_DELAY] = m_NetInputBuffer;
			}

			m_UpdatePullInput = false;
		}

		const int latestLocalFrameDelta = m_Gamestate.FrameCount - m_LatestNetworkFrame;
		m_LocalFrameDelta = latestLocalFrameDelta;

		NetworkInputPackedge toSendPackedge;
		//Prepare the network package to send to the opponent
		const int startInputIndex = m_Gamestate.FrameCount - NET_PACKET_INPUT_HISTORY_SIZE + 1 + NETWORK_INPUT_DELAY;
		//Fill the network package's input history with our local input.
		for (int i = 0; i < NET_PACKET_INPUT_HISTORY_SIZE; i++)
		{
			const int inputIndex = startInputIndex + i;
			if (inputIndex >= 0)
			{
				toSendPackedge.InputHistory[i] = GetInputHistory(m_LocalSide, inputIndex);
			}
			else
			{
				//Sential value for inputs we don't use;
				toSendPackedge.InputHistory[i] = UINT_MAX;
			}
		}

		//Indicated the last frame the final input in the buffer is designated for;
		toSendPackedge.FrameCount = m_Gamestate.FrameCount + NETWORK_INPUT_DELAY;

		//Send frame delta (time delta) to the other player.
		toSendPackedge.FrameDelta = latestLocalFrameDelta;

		bool bRecievedInput = false;
		if (m_NetworkState == NetworkState::Host) m_LatestInputPakedge = TickNetworkHost(toSendPackedge, bRecievedInput);
		if (m_NetworkState == NetworkState::Client) m_LatestInputPakedge = TickNetworkClient(toSendPackedge, bRecievedInput);

		if (bRecievedInput == true)
		{
			//DFG_TRACE("Received Net Input for Frame: {0}", m_LatestInputPakedge.FrameCount);

			const int startFrame = m_LatestInputPakedge.FrameCount - NET_PACKET_INPUT_HISTORY_SIZE + 1;
			for (int i = 0; i < NET_PACKET_INPUT_HISTORY_SIZE; i++)
			{
				//Check if we recived input for a new frame;
				const int CheckFrame = startFrame + i;
				if (CheckFrame == m_LatestNetworkFrame + 1)
				{
					// Advance the network frame so we know that we have inputs for the simulation
					m_LatestNetworkFrame++;
				}

				//DFG_TRACE("Input: {0}, Frame: {1}", m_LatestInputPakedge.InputHistory[i], (int)(m_LatestInputPakedge.FrameCount - NET_PACKET_INPUT_HISTORY_SIZE + i + 1));

				//Record the other players input to be used in the game simulation;
				if (startFrame + i > 0)
					m_InputHistory[m_OpponentSide][startFrame + i] = m_LatestInputPakedge.InputHistory[i];
			}
		}

		bool bUpdateNextFrame = (m_Gamestate.FrameCount == 0);

		if (m_NetworkState == NetworkState::None)
		{
			bUpdateNextFrame = false;
		}

		//Allow game simulation to run when we are not in network mode.
		if (m_BootNetworkSetting == BootNetworkSetting::NoConnection)
		{
			bUpdateNextFrame = true;
		}

		if (m_NetworkSyncType == NetworkSyncType::Rollback)
		{
			//Limit how far ahead we can get ahead of the opponent.
			bUpdateNextFrame = m_Gamestate.FrameCount < m_LatestNetworkFrame + MAX_ROLLBACK_FRAMES;

			//Check the frame delta difference of the local client and the remote in order to sync clocks.
			if (latestLocalFrameDelta - m_LatestInputPakedge.FrameDelta > 1)
			{
				bUpdateNextFrame = false;
			}	
		}
		else if (m_NetworkSyncType == NetworkSyncType::LockStep)
		{
			//Only update the game simulation when we have input for the target frame.
			if (m_LatestNetworkFrame > m_Gamestate.FrameCount)
			{
				bUpdateNextFrame = true;
			}
		}

		//Indicates whether or not we are resimulating the game after a rollback restore event;
		bool bIsResimulating = false;

		//The number of times we need to run the game simulation
		int simulationFrames = 1;

		//Detect if we have new input so we can resimulate the game (rollback)
		if (m_LatestNetworkFrame > m_LastSavedGameStateFrame)
		{
			bIsResimulating = true;
			bUpdateNextFrame = true;

			//Calculate the number of frames we need to resimulate plus the current frame.
			simulationFrames = m_Gamestate.FrameCount - m_LastSavedGameStateFrame;

			//DFG_TRACE("Rollback: Frames: {0}, LatestNetworkFrame: {1}, LastSavedGameStateFrame: {2}", simulationFrames, m_LatestNetworkFrame, m_LastSavedGameStateFrame);

			//Restore the latest saved game state.
			memcpy(&m_Gamestate, &m_SavedGameState, sizeof(SimulatonState));
		}

		if (Input::IsKeyPressed(Key::KEY_PAUSE))
		{
			bUpdateNextFrame = false;
		}

		if (bUpdateNextFrame == true)
		{
			DFG_SYNC_TRACE("Frame: {0}, P1: {1}, P2: {2}", m_Gamestate.FrameCount, m_Gamestate.Inputs[0], m_Gamestate.Inputs[1]);
			//Run the game simulation the number of times previously determined
			for (int simFrame = 0; simFrame < simulationFrames; simFrame++)
			{

				//DFG_TRACE("Ticked game frame: {0}", m_Gamestate.FrameCount);

				//Assign the local players input with delay applied.
				m_Gamestate.Inputs[m_LocalSide] = GetInputHistory(m_LocalSide, m_Gamestate.FrameCount);

				//Assign our opponents input from the network.
				m_Gamestate.Inputs[m_OpponentSide] = GetInputHistory(m_OpponentSide, m_Gamestate.FrameCount);

				//DFG_SYNC_TRACE("Frame: {0}, P1: {1}, P2: {2}", m_Gamestate.FrameCount, m_Gamestate.Inputs[0], m_Gamestate.Inputs[1]);

				UpdateSimulation(m_Gamestate);

				//Indicate we can record input for the next frame.
				m_UpdatePullInput = true;

				m_Gamestate.FrameCount++;

				if (bIsResimulating == true)
				{
					//Save game state when we have input for this frame;
					if (m_LatestNetworkFrame >= m_Gamestate.FrameCount - 1)
					{
						m_LastSavedGameStateFrame = m_Gamestate.FrameCount - 1;
						memcpy(&m_SavedGameState, &m_Gamestate, sizeof(SimulatonState));
					}
				}

				//If we are ahead of the latest confirmed input frame track the number of forward sim frames.
				//if (m_Gamestate.FrameCount > m_LatestNetworkFrame)
				//{
				//	m_OutOfSyncFramees++;
				//}
			}	//
		}
	}

	void Game::UpdateSimulation(SimulatonState& gameState)
	{
		UpdateEntity(gameState.Entities[0], gameState.Inputs[0]);
		UpdateEntity(gameState.Entities[1], gameState.Inputs[1]);
	}

	void Game::UpdateEntity(EntityState& entity, uint32_t input)
	{
		entity.Velocity.x = 0;
		entity.Velocity.y = 0;

		if (input & (uint32_t)InputCommand::Up) entity.Velocity.y = 10;
		if (input & (uint32_t)InputCommand::Down) entity.Velocity.y -= 10;
		if (input & (uint32_t)InputCommand::Left) entity.Velocity.x -= 10;
		if (input & (uint32_t)InputCommand::Right) entity.Velocity.x = 10;

		entity.Position.x += entity.Velocity.x;
		entity.Position.y += entity.Velocity.y;
	}

	void Game::OnRender()
	{
		RenderEntity(m_Gamestate.Entities[0], glm::vec4(1, 0, 0, 1));
		RenderEntity(m_Gamestate.Entities[1], glm::vec4(0, 0, 1, 1));
	}

	void Game::OnUIRender()
	{
		Renderer2D::s_TextRenderer.DrawText("P1: [" + std::to_string(m_Gamestate.Entities[0].Position.x) + ", " + std::to_string(m_Gamestate.Entities[0].Position.y) + "]", glm::vec2(20, Application::Get().GetWindow().GetHeight() - 40));
		Renderer2D::s_TextRenderer.DrawText("P2: [" + std::to_string(m_Gamestate.Entities[1].Position.x) + ", " + std::to_string(m_Gamestate.Entities[1].Position.y) + "]", glm::vec2(20, Application::Get().GetWindow().GetHeight() - 60));
		Renderer2D::s_TextRenderer.DrawText("Frame Count: " + std::to_string(m_Gamestate.FrameCount), glm::vec2(20, Application::Get().GetWindow().GetHeight() - 80));
		Renderer2D::s_TextRenderer.DrawText("Net Frame: " + std::to_string(m_LatestInputPakedge.FrameCount), glm::vec2(20, Application::Get().GetWindow().GetHeight() - 100));
		Renderer2D::s_TextRenderer.DrawText("LatestInputFrame: " + std::to_string(m_LatestNetworkFrame), glm::vec2(20, Application::Get().GetWindow().GetHeight() - 120));
		Renderer2D::s_TextRenderer.DrawText("LocalFrameDelta: " + std::to_string(m_LocalFrameDelta), glm::vec2(20, Application::Get().GetWindow().GetHeight() - 140));
	}

	void Game::HandleInput()
	{
		if (Input::IsKeyPressed(Key::KEY_W)) m_NetInputBuffer |= (uint32_t)InputCommand::Up;
		if (Input::IsKeyPressed(Key::KEY_S)) m_NetInputBuffer |= (uint32_t)InputCommand::Down;
		if (Input::IsKeyPressed(Key::KEY_A)) m_NetInputBuffer |= (uint32_t)InputCommand::Left;
		if (Input::IsKeyPressed(Key::KEY_D)) m_NetInputBuffer |= (uint32_t)InputCommand::Right;
	}

	void Game::RenderEntity(EntityState entity, glm::vec4 color)
	{
		Renderer2D::DrawQuad(glm::vec2(entity.Position.x * 0.01, entity.Position.y * 0.01), glm::vec2{ 1,1 }, color);
	}

	uint32_t Game::GetInputHistory(uint32_t side, uint32_t frameCount)
	{
		return m_InputHistory[side][frameCount];
	}
}
