#pragma once

#include "DFGEngine/Core/Base.h"
#include "DFGEngine/Core/Timestep.h"
#include "DFGEngine/Events/Event.h"

namespace DFGEngine 
{
	class Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnEvent(Event& event) {}

		const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};
}