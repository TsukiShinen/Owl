#pragma once

#include "Event.h"

namespace Owl
{
	class WindowResizeEvent final : public Event
	{
	public:
		WindowResizeEvent(const unsigned int pWidth, const unsigned int pHeight)
			: m_Width(pWidth), m_Height(pHeight)
		{
		}

		~WindowResizeEvent() override = default;

		unsigned int GetWidth() const { return m_Width; }
		unsigned int GetHeight() const { return m_Height; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

	private:
		unsigned int m_Width, m_Height;
	};

	class WindowCloseEvent final : public Event
	{
	public:
		WindowCloseEvent() = default;
		~WindowCloseEvent() override = default;

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppTickEvent final : public Event
	{
	public:
		AppTickEvent() = default;
		~AppTickEvent() override = default;

		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppUpdateEvent final : public Event
	{
	public:
		AppUpdateEvent() = default;
		~AppUpdateEvent() override = default;

		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppRenderEvent final : public Event
	{
	public:
		AppRenderEvent() = default;
		~AppRenderEvent() override = default;

		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
}
