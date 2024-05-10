#pragma once

#include "Owl/Events/Event.h"

namespace Owl
{
	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(std::string pTitle = "Owl Engine", const unsigned int pWidth = 1280,
		            const unsigned int pHeight = 720)
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

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFunction& pCallback) = 0;
		virtual void SetVSync(bool pEnabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() = 0;

		static Window* Create(const WindowProps& pProps = WindowProps());
	};
}
