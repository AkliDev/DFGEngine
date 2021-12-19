#pragma once

#include "DFGEngine.h"

namespace DFGEngine
{
	class Sandbox2D : public Layer
	{
	public:
		Sandbox2D();
		virtual ~Sandbox2D() = default;

		void OnAttach() override;
		void OnDetach() override;

		void OnUpdate() override;

		void OnEvent(Event& e) override;
	};
}