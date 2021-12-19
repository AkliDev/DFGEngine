#pragma once

#include "Event.h"

#include <string.h>

namespace DFGEngine
{
	class TextEvent : public Event
	{
	public:
		inline std::string GetText() const { return m_Text; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		TextEvent(std::string text)
			: m_Text(text) {}

		std::string m_Text;
	};

	class TextInputEvent : public TextEvent
	{
	public:
		TextInputEvent(const std::string text)
			: TextEvent(text) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "TextInputEvent: " << m_Text;
			return ss.str();
		}

		EVENT_CLASS_TYPE(TextInput)
	};
}
