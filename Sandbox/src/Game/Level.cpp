#include "Level.h"

#include <fstream>
#include <sstream>

namespace DFGEngine
{	
	void Level::Load(const char* file, float levelWidth, float levelHeight)
	{
		// clear old data
		this->m_Bricks.clear();
		// load from file
		unsigned int tileCode;
		Level level;
		std::string line;
		std::ifstream fstream(file);
		std::vector<std::vector<unsigned int>> tileData;
		if (fstream)
		{
			while (std::getline(fstream, line)) // read each line from level file
			{
				std::istringstream sstream(line);
				std::vector<unsigned int> row;
				while (sstream >> tileCode) // read each word separated by spaces
				{
					row.push_back(tileCode);
				}
				tileData.push_back(row);
			}
			if (tileData.size() > 0)
			{
				Init(tileData, levelWidth, levelHeight);
			}
		}
	}

	void Level::OnRender()
	{
		for (Ref<Brick> brick : m_Bricks)
		{
			if (brick->IsDestroyed() == false)
			{
				brick->OnRender();
			}
		}
	}

	void Level::Reset()
	{
		for (Ref<Brick> brick : m_Bricks)
		{
			if (brick->IsDestroyed() == true)
			{
				brick->Destroyed(false);
			}
		}
	}

	bool Level::IsCompleted()
	{
		for (Ref<Brick> brick : m_Bricks)
		{
			if (brick->IsSolid() == false && brick->IsDestroyed() == false)
			{
				return false;
			}
		}
		return true;
	}

	void Level::Init(std::vector<std::vector<uint32_t>> tileData, float levelWidth, float levelHeight)
	{
		// calculate dimensions
		unsigned int height = tileData.size();
		unsigned int width = tileData[0].size();
		float unitWidth = levelWidth / width;
		float unitHeight = levelHeight / height;
		// initialize level tiles based on tileData		
		for (uint32_t y = 0; y < height; ++y)
		{
			for (uint32_t x = 0; x < width; ++x)
			{
				glm::vec3 pos(((unitWidth) * x) + unitWidth * 0.5f, levelHeight * 2.0f -unitHeight * y, 0.0f);
				glm::vec3 scale(unitWidth, unitHeight, 1.0f);
				glm::vec4 color(0.8f, 0.8f, 0.7f, 1.0f);

				// check block type from level data (2D level array)
				if (tileData[y][x] == 1) // solid
				{
					Ref<Brick> brick = CreateRef<Brick>(pos, scale, Renderer2D::s_TextureLibrary.Get("block_solid"), color);
					brick->Solid(true);
					this->m_Bricks.push_back(brick);
				}
				else if (tileData[y][x] > 1)
				{
					if (tileData[y][x] == 2)
						color = glm::vec4(0.2f, 0.6f, 1.0f, 1.0f);
					else if (tileData[y][x] == 3)
						color = glm::vec4(0.0f, 0.7f, 0.0f, 1.0f);
					else if (tileData[y][x] == 4)
						color = glm::vec4(0.8f, 0.8f, 0.4f, 1.0f);
					else if (tileData[y][x] == 5)
						color = glm::vec4(1.0f, 0.5f, 0.0f, 1.0f);
	
					Ref<Brick> brick = CreateRef<Brick>(pos, scale, Renderer2D::s_TextureLibrary.Get("block"), color);
					m_Bricks.push_back(brick);
				}
			}
		}
	}
}