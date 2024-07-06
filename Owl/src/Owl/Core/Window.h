#pragma once

#include "Owl/Events/Event.h"

namespace Owl
{
	struct WindowProps
	{
		std::string Title;
		uint32_t  Width;
		uint32_t  Height;

		WindowProps(std::string pTitle = "Owl Engine", const uint32_t  pWidth = 1920,
		            const uint32_t pHeight = 1080)
			: Title(std::move(pTitle)), Width(pWidth), Height(pHeight)
		{
		}
	};

	class Window
	{
	public:
		using EventCallbackFunction = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual uint32_t  GetWidth() const = 0;
		virtual uint32_t  GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFunction& pCallback) = 0;
		virtual void SetVSync(bool pEnabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() = 0;

		static Scope<Window> Create(const WindowProps& pProps = WindowProps());
	};
}
