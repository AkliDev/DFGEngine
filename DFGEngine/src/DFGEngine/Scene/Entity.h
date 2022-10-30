#pragma once
#include "Scene.h"

namespace DFGEngine
{
	class Entity
	{
	public:
		Entity(Scene* scene);
	private:
		Scene* m_Scene = nullptr;
	};
}