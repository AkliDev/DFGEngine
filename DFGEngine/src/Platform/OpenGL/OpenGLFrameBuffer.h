#pragma once

#include "DFGEngine/Renderer/FrameBuffer.h"
#include "OpenGLTexture.h"

namespace DFGEngine
{
	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(const FramebufferSpecification& specification);
		virtual ~OpenGLFrameBuffer();

		void Invalidate();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(const uint32_t width, const uint32_t height) override;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;

		virtual void ClearAttachment(uint32_t attachmentIndex, int value) override;

		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override { DFG_CORE_ASSERT(index < m_ColorAttachmentIDs.size()); return m_ColorAttachmentIDs[index]; }
		virtual const Ref<Texture2D> GetColorAttachmentAsTexture(uint32_t index = 0) const override { DFG_CORE_ASSERT(index < m_ColorAttachmentTextures.size()); return m_ColorAttachmentTextures[index]; };

		virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; }
	private:
		uint32_t m_RendererID = 0;

		FramebufferSpecification m_Specification;

		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
		FramebufferTextureSpecification m_DepthAttachmentSpecification = FramebufferTextureFormat::None;

		std::vector<uint32_t> m_ColorAttachmentIDs;
		uint32_t m_DepthAttachmentID;

		std::vector<Ref<OpenGLTexture2D>> m_ColorAttachmentTextures;
	};
}
