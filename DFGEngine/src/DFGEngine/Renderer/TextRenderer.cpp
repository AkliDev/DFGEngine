#include "dfgpch.h"
#include "TextRenderer.h"
#include "Renderer2D.h"

#include <ft2build.h>
#include FT_FREETYPE_H


namespace DFGEngine
{	
	void TextRenderer::Load(const std::string& font, uint32_t fontSize)
	{
		m_CharactersMap.clear();

		
		FT_Library ft;
		uint32_t result = FT_Init_FreeType(&ft);
		DFG_ASSERT(ft > 0, "ERROR::FREETYPE: Could not init FreeType Library");

		FT_Face face;
		result = FT_New_Face(ft, font.c_str(), 0, &face);
		DFG_ASSERT(true, "ERROR::FREETYPE: Failed to load font");

		FT_Set_Pixel_Sizes(face, 0, fontSize);

		//For the first 128 ASCII characters, pre-load/compile their characters and store them.
		for (unsigned char c = 0; c < 128; c++)
		{
			// load character glyph 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				DFG_CORE_CRITICAL("ERROR::FREETYTPE: Failed to load Glyph");
				continue;
			}
			
			Ref<Texture2D> texture = Texture2D::Create(face);

			// now store character for later use
			Character character = {
				texture,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				face->glyph->advance.x
			};
			m_CharactersMap.insert(std::pair<char, Character>(c, character));
		}
		// destroy FreeType once we're finished
		FT_Done_Face(face);
		FT_Done_FreeType(ft);
	}

	void TextRenderer::DrawText(const std::string& text, glm::vec2 position, float scale, glm::vec4 color)
	{
		// iterate through all characters
		std::string::const_iterator c;
		for (c = text.begin(); c != text.end(); c++)
		{
			Character ch = m_CharactersMap[*c];

			float xpos = position.x;
			float ypos = position.y;

			xpos = position.x + ch.Bearing.x * scale;
			ypos = position.y - (ch.Size.y - ch.Bearing.y) * scale;

			float w = ch.Size.x * scale;
			float h = ch.Size.y * scale;

			//Vertex data for quad used to render text.
			glm::vec3 textVertexPositions[4];
			textVertexPositions[0] = { xpos,		ypos,	  0.1f };
			textVertexPositions[1] = { xpos + w,	ypos,	  0.1f };
			textVertexPositions[2] = { xpos + w,	ypos + h, 0.1f };
			textVertexPositions[3] = { xpos,		ypos + h, 0.1f };

			Renderer2D::DrawText(textVertexPositions, ch.Texture, 1.0f, color);

			position.x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (1/64th times 2^6 = 64)
		}
	}

	void TextRenderer::DrawText(const std::string& text, glm::vec3 position, float scale, glm::vec4 color)
	{
		// iterate through all characters
		std::string::const_iterator c;
		for (c = text.begin(); c != text.end(); c++)
		{
			Character ch = m_CharactersMap[*c];

			float xpos = position.x;
			float ypos = position.y;

			xpos = position.x + ch.Bearing.x * scale;
			ypos = position.y - (ch.Size.y - ch.Bearing.y) * scale;

			float w = ch.Size.x * scale;
			float h = ch.Size.y * scale;

			//Vertex data for quad used to render text.
			glm::vec3 textVertexPositions[4];
			textVertexPositions[0] = { xpos,		ypos,	  position.z };
			textVertexPositions[1] = { xpos + w,	ypos,	  position.z };
			textVertexPositions[2] = { xpos + w,	ypos + h, position.z };
			textVertexPositions[3] = { xpos,		ypos + h, position.z };

			Renderer2D::DrawText(textVertexPositions, ch.Texture, 1.0f, color);
			
			position.x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (1/64th times 2^6 = 64)
		}
	}
}