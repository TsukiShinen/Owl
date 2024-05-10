#pragma once

#include "Owl/Core/Base.h"

namespace Owl
{
	enum class EventType
	{
		None,
		// Window Events
		WindowClose,
		WindowResize,
		WindowLostFocus,
		WindowMoved,
		// Application Events
		AppTick,
		AppUpdate,
		AppRender,
		// Keyboard Events
		KeyPressed,
		KeyReleased,
		// Mouse Events
		MouseButtonPressed,
		MouseButtonReleased,
		MouseMoved,
		MouseScrolled
	};

	enum EventCategory
	{
		None                     = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput       = BIT(1),
		EventCategoryKeyboard    = BIT(2),
		EventCategoryMouse       = BIT(3),
		EventCategoryMouseButton = BIT(4),
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
								EventType GetEventType() const override { return GetStaticType(); }\
								const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) int GetCategoryFlags() const override { return category; }

	class Event
	{
		friend class EventDispatcher;

	public:
		virtual ~Event() = default;

		bool IsHandled = false;

		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		bool IsInCategory(const EventCategory pCategory) const
		{
			return GetCategoryFlags() & pCategory;
		}
	};

	class EventDispatcher
	{
	public:
		EventDispatcher(Event& pEvent)
			: m_Event(pEvent)
		{
		}

		template <typename T, typename F>
		bool Dispatch(const F& pFunction)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.IsHandled = pFunction(static_cast<T&>(m_Event));
				return true;
			}
			return false;
		}

	private:
		Event& m_Event;
	};

	inline std::ostream& operator <<(std::ostream& pOs, const Event& pEvent)
	{
		return pOs << pEvent.ToString();
	}
}
