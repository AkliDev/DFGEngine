#pragma once
#include "Texture.h"

#include <glm/glm.hpp>
#include <map>

namespace DFGEngine
{
	/// Holds all state information relevant to a character as loaded using FreeType
	struct Character {
		Ref<Texture2D> Texture;
		glm::ivec2 Size;// size of glyph
		glm::ivec2 Bearing;// offset from baseline to left/top of glyph
		uint32_t Advance;// horizontal offset to advance to next glyph
	};

	class TextRenderer
	{
	public:
		// holds a list of pre-compiled Characters
		std::map<char, Character> m_CharactersMap;
		// pre-compiles a list of characters from the given font
		void Load(const std::string& font, uint32_t fontSize);
		// renders a string of text using the pre-compiled list of characters
		void DrawText(const std::string& text, glm::vec2 position, float scale = 0.4f, glm::vec4 color = glm::vec4(1.0f));
		void DrawText(const std::string& text, glm::vec3 position, float scale = 0.4f, glm::vec4 color = glm::vec4(1.0f));
	};
}