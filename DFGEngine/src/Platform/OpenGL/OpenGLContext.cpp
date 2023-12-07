#include "dfgpch.h"
#include "OpenGLContext.h"

#include <SDL.h>
#include <glad/glad.h>

namespace DFGEngine
{
    OpenGLContext::OpenGLContext(SDL_Window* windowHandle)
        :m_WindowHandle(windowHandle)
    {
        DFG_CORE_ASSERT(windowHandle, "Window handle is null!")
    }

    void OpenGLContext::Init()
    {
        //DFG_PROFILE_FUNCTION();

        SDL_GL_LoadLibrary(NULL); // Default OpenGL is fine.

        // Request an OpenGL 4.6 context (should be core)
        SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
        // Also request a depth buffer
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
        
#if defined(DFG_DEBUG)
        //enable debug context
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif

        m_Context = SDL_GL_CreateContext(m_WindowHandle);
        SDL_GL_MakeCurrent(m_WindowHandle, m_Context);

        int status = gladLoadGLLoader(SDL_GL_GetProcAddress);
        DFG_CORE_ASSERT(status, "failed to initialize Glad!");

        DFG_CORE_INFO("OpenGL Info:");
        DFG_CORE_INFO("  Vendor:    {0}", (char*)glGetString(GL_VENDOR));
        DFG_CORE_INFO("  Renderer:  {0}", (char*)glGetString(GL_RENDERER));
        DFG_CORE_INFO("  Version:   {0}", (char*)glGetString(GL_VERSION));

        DFG_CORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 6), "Alternate requires at least OpenGL version 4.6!");
    }

    void OpenGLContext::SwapBuffer()
    {
        //DFG_PROFILE_FUNCTION();

        SDL_GL_SwapWindow(m_WindowHandle);      
    }

    void OpenGLContext::DeleteRenderContext()
    {
        SDL_GL_DeleteContext(m_Context);
    }
}
