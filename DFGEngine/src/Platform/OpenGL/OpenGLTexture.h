#pragma once
#include "DFGEngine/Renderer/Texture.h"

// TODO: REMOVE!
typedef unsigned int GLenum;

namespace DFGEngine
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const uint32_t width, const uint32_t height);
		OpenGLTexture2D(const std::string& path);
		OpenGLTexture2D(const uint32_t colorAttachmentID, const uint32_t width, const uint32_t height, const GLenum m_InternalFormat, const GLenum m_DataFormat);
		
		virtual ~OpenGLTexture2D();
	
		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual uint32_t GetRendererID() const override { return m_RendererID; }
		virtual const std::string& GetName() const override { return m_Path; };

		virtual void SetData(void* data, uint32_t size) override;
	
		virtual void Bind(uint32_t slot = 0) const override;

		virtual bool operator==(const Texture& other) const override
		{
			return m_RendererID == other.GetRendererID();
		}
	public:
		void InvalidateTextureID() { DFG_CORE_TRACE("Invalidated textureID {0}", m_RendererID); m_RendererID = 0; };
	
	private:
		std::string m_Name;
		std::string m_Path;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
		GLenum m_InternalFormat, m_DataFormat;
	};
}