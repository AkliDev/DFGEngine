#pragma once
#include "DFGEngine.h"
#include "Brick.h"
#include <vector>

namespace DFGEngine
{
	class Level
	{
	public:
		Level() = default;

		void Load(const char* file, float levelWidth, float levelHeight);
		void OnRender();
		void Reset();
		
		bool IsCompleted();

		std::vector<Ref<Brick>>& GetBricks() { return m_Bricks; }	
	private:
		// initialize level from tile data
		void Init(std::vector<std::vector<uint32_t>> tileData, float levelWidth, float levelHeight);
	private:
		std::vector<Ref<Brick>> m_Bricks;
	};
}