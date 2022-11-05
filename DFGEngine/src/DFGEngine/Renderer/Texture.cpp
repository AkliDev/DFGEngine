#include "dfgpch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace DFGEngine
{
	Ref<Texture2D> Texture2D::Create(const uint32_t width, const uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:	DFG_CORE_ASSERT(false, "RendererAPI::None is currently not supported!") return nullptr;
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLTexture2D>(width, height);
		}

		DFG_CORE_ASSERT(false, "Unkown RendererAPI!")
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	DFG_CORE_ASSERT(false, "RendererAPI::None is currently not supported!") return nullptr;
			case RendererAPI::API::OpenGL:	return CreateRef<OpenGLTexture2D>(path);
		}

		DFG_CORE_ASSERT(false, "Unkown RendererAPI!")
		return nullptr;
	}

	DFGEngine::Ref<DFGEngine::Texture2D> Texture2D::Create(const FT_Face& face)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:	DFG_CORE_ASSERT(false, "RendererAPI::None is currently not supported!") return nullptr;
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLTexture2D>(face);
		}

		DFG_CORE_ASSERT(false, "Unkown RendererAPI!")
			return nullptr;
	}

	void TextureLibrary::Add(const std::string& name, const Ref<Texture2D>& texture)
	{
		DFG_ASSERT(!Exists(name), "Texture already exists!");
		m_Textures[name] = texture;
	}

	void TextureLibrary::Add(const Ref<Texture2D>& texture)
	{
		auto& name = texture->GetName();
		Add(name, texture);
	}

	Ref<Texture2D> TextureLibrary::Load(const std::string& name, const std::string& filePath)
	{
		auto& texture = Texture2D::Create(filePath);
		Add(name, texture);
		return texture;
	}

	Ref<Texture2D> TextureLibrary::Get(const std::string& name)
	{
		DFG_ASSERT(Exists(name), "Texture not found!");
		return m_Textures[name];
	}

	bool TextureLibrary::Exists(const std::string& name) const
	{
		return m_Textures.find(name) != m_Textures.end();
	}
}
