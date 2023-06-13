#pragma once
#include "DFGEngine.h"
#include <rttr/registration>

namespace DFGEngine
{
	static void HelloWorld(int32_t x, int32_t y) { DFG_TRACE("HELLO WORLD ({0}, {1})",x,y); }
	static int32_t Add(int32_t x, int32_t y) { return x + y; }

	class Object
	{
	public:
		Object() {}
		~Object() {}
		virtual int32_t OnRender(int32_t x, int32_t y, int32_t w, int32_t h)
		{
			Renderer2D::DrawQuad({x,y}, { w,h }, Renderer2D::s_TextureLibrary.Get("face"));
			return x + y;
		}
	public:
		glm::vec2 m_Position{0};
	};

	RTTR_REGISTRATION
	{
		rttr::registration::method("HelloWorld", &HelloWorld);
		rttr::registration::method("Add", &Add);

		rttr::registration::class_<Object>("Object")
			.constructor()
			.method("OnRender", &Object::OnRender)
			.property("Position", &Object::m_Position);
	}
}