#pragma once
#include "DFGEngine.h"
#include "Network.h"

namespace DFGEngine
{
	const int INPUT_HISTORY_SIZE = 3*60*60;
	const int NETWORK_INPUT_DELAY = 1;
	const int MAX_ROLLBACK_FRAMES = 10;

	enum class BootNetworkSetting
	{
		NoConnection,
		Host,
		Client,
	};

	enum class NetworkState
	{
		None,
		Host,
		Client,
	};

	enum class NetworkSyncType
	{
		Rollback,
		LockStep,
	};

	enum class InputCommand : uint32_t
	{
		None = 0,
		Up = 1,
		Down = 2,
		Left = 4,
		Right = 8
	};

	struct Vector2i
	{
		int32_t x{ 0 };
		int32_t y{ 0 };
	};

	struct EntityState
	{
		Vector2i Position;
		Vector2i Velocity;
	};

	struct SimulatonState
	{
		int FrameCount = 0;
		EntityState Entities[2];
		uint32_t Inputs[2];
	};

	class Game
	{
	public:
		void Init();
		void HandleInput();
		void OnUpdate();
		void UpdateSimulation(SimulatonState& gameState);
		void UpdateEntity(EntityState& entity, uint32_t input);
		void OnRender();
		void OnUIRender();
	
	private:
		void RenderEntity(EntityState entity, glm::vec4 color);
		uint32_t GetInputHistory(uint32_t side, uint32_t frameCount);

		bool m_UpdatePullInput = true;

		SimulatonState m_Gamestate;
		SimulatonState m_SavedGameState;	

		uint32_t m_LocalSide = 0;
		uint32_t m_OpponentSide = 1;

		BootNetworkSetting m_BootNetworkSetting = BootNetworkSetting::NoConnection;
		NetworkState m_NetworkState = NetworkState::None;
		NetworkSyncType m_NetworkSyncType = NetworkSyncType::Rollback;

		int m_LocalFrameDelta = 0;
		
		uint32_t m_NetInputBuffer = 0;
		uint32_t m_InputHistoryIndex = 0;

		NetworkInputPackedge m_LatestInputPakedge;
		//Indicates the most recent confirmed frame from the other player received inputs
		int m_LatestNetworkFrame = -1;
		//Record the frame number of the last game state frame we saved
		int m_LastSavedGameStateFrame = -1;

		// Records the input history for every frame for both player 1 and 2 locally
		uint32_t m_InputHistory[2][INPUT_HISTORY_SIZE] = { 0 };
	};
}