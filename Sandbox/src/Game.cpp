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
	}

	void Game::OnUpdate()
	{
		
		m_Gamestate.Inputs[0] = (uint32_t)InputCommand::None;
		m_Gamestate.Inputs[1] = (uint32_t)InputCommand::None;

		if (m_NetworkState == NetworkState::None)
		{
			if (Input::IsKeyPressed(Key::KEY_F1))
			{
				m_LocalSide = 0;
				m_OpponentSide = 1;
				m_NetworkState = NetworkState::Host;
				InitializeHost();
			}
			if (Input::IsKeyPressed(Key::KEY_F3))
			{
				m_LocalSide = 1;
				m_OpponentSide = 0;
				m_NetworkState = NetworkState::Client;
				InitializeClient();
			}
		}

		m_NetInputBuffer = (uint32_t)InputCommand::None;
		HandleInput();

		//Record this input in the history buffer for sending the the other player.
		if (m_FrameCount < INPUT_HISTORY_SIZE - NETWORK_INPUT_DELAY)
		{
			m_InputHistory[m_LocalSide][m_FrameCount + NETWORK_INPUT_DELAY] = m_NetInputBuffer;
		}

		bool bRecievedInput = false;

		NetworkInputPackedge toSendPackedge;	
		//Prepare the network package to send to the opponent
		const int startInputIndex = m_FrameCount - NET_PACKET_INPUT_HISTORY_SIZE + 1 + NETWORK_INPUT_DELAY;
		//Fill the network package's input history with our local input.
		for (int i =0; i < NET_PACKET_INPUT_HISTORY_SIZE; i++)
		{
			const int inputIndex = startInputIndex + i;
			if (inputIndex >= 0)
			{
				toSendPackedge.InputHistory[i] = m_InputHistory[m_LocalSide][inputIndex];
			}
			else
			{
				//Sential value for inputs we don't use;
				toSendPackedge.InputHistory[i] = UINT_MAX;
			}
		}

		//Indicated the last frame;
		toSendPackedge.FrameCount = m_FrameCount + NETWORK_INPUT_DELAY;

		if (m_NetworkState == NetworkState::Host) m_LatestInputPakedge = TickNetworkHost(toSendPackedge, bRecievedInput);
		if (m_NetworkState == NetworkState::Client) m_LatestInputPakedge = TickNetworkClient(toSendPackedge, bRecievedInput);

		//for (const auto& InputPackedge : m_NetworkInputHistory)
		//{
		//	if (InputPackedge.FrameCount == m_LatestInputPakedge.FrameCount)
		//	{
		//		bRecievedInput = false;
		//		break;
		//	}
		//}

		if (bRecievedInput == true)
		{
			DFG_TRACE("Received Net Input for Frame: {0}", m_LatestInputPakedge.FrameCount);

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
				
				DFG_TRACE("Input: {0}, Frame: {1}", m_LatestInputPakedge.InputHistory[i], (int)(m_LatestInputPakedge.FrameCount - NET_PACKET_INPUT_HISTORY_SIZE + i + 1));

				//Record the other players input to be used in the game simulation;
				if(startFrame + i > 0)
				m_InputHistory[m_OpponentSide][startFrame + i] = m_LatestInputPakedge.InputHistory[i];
			}
		}

		bool bUpdateNextFrame = (m_FrameCount == 0);
		
		//Only update the game simulation when we have input for the target frame.
		if (m_LatestNetworkFrame > m_FrameCount)bUpdateNextFrame = true;
		if(m_NetworkState == NetworkState::None) bUpdateNextFrame = false;

		if (bUpdateNextFrame == true)
		{
			DFG_TRACE("Ticked game frame: {0}", m_FrameCount);
			//Assign the local players input with delay applied.
			m_Gamestate.Inputs[m_LocalSide] = m_InputHistory[m_LocalSide][m_FrameCount + NETWORK_INPUT_DELAY];
			//Assign our opponents input from the network.
			m_Gamestate.Inputs[m_OpponentSide] = m_InputHistory[m_OpponentSide][m_FrameCount];
			UpdateSimulation(m_Gamestate);
			m_FrameCount++;
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

		if (input & (uint32_t)InputCommand::Up) entity.Velocity.y = 3;
		if (input & (uint32_t)InputCommand::Down) entity.Velocity.y -= 3;
		if (input & (uint32_t)InputCommand::Left) entity.Velocity.x -= 3;
		if (input & (uint32_t)InputCommand::Right) entity.Velocity.x = 3;

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
		Renderer2D::s_TextRenderer.DrawText("Frame Count: " + std::to_string(m_FrameCount), glm::vec2(20, Application::Get().GetWindow().GetHeight() - 80));
		Renderer2D::s_TextRenderer.DrawText("Net Frame: " + std::to_string(m_LatestInputPakedge.FrameCount), glm::vec2(20, Application::Get().GetWindow().GetHeight() - 100));
		Renderer2D::s_TextRenderer.DrawText("LatestInputFrame: " + std::to_string(m_LatestNetworkFrame), glm::vec2(20, Application::Get().GetWindow().GetHeight() - 120));
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
}
