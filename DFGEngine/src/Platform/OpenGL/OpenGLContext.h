#pragma once

#include "DFGEngine/Renderer/GraphicsContext.h"

struct SDL_Window;

namespace DFGEngine
{
    class OpenGLContext : public GraphicsContext
    {
    public:
        OpenGLContext(SDL_Window* windowHandle);

        virtual void* GetRenderContext() override { return m_Context; };

        virtual void Init() override;
        virtual void SwapBuffer() override;
        virtual void DeleteRenderContext() override;

    private:
        SDL_Window* m_WindowHandle;
        void* m_Context;
    };
}