#pragma once

#include "DFGEngine.h"

class Sandbox2D : public DFGEngine::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	void OnAttach() override;
	void OnDetach() override;

	void OnUpdate() override;
};