#pragma once
#include "DFGEngine/Core/Base.h"
#include <string>
#include <vector>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace DFGEngine
{
	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetRendererID() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;

		virtual bool operator==(const Texture& other) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(const uint32_t width, const uint32_t height);
		static Ref<Texture2D> Create(const std::string& path);
		static Ref<Texture2D> Create(const FT_Face& face);
	};

	class TextureLibrary
	{
	public:
		~TextureLibrary();
		void Add(const std::string& name, const Ref<Texture2D>& texture);
		Ref<Texture2D> Load(const std::string& name, const std::string& filePath);

		Ref<Texture2D> Get(const std::string& name);

		bool Exists(const std::string& name) const;
	private:
		std::unordered_map<std::string, Ref<Texture2D>> m_Textures;
	};
}