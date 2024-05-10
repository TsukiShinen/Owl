#pragma once

#include "Owl/Core/Window.h"

#include <GLFW/glfw3.h>

#include "Owl/Renderer/GraphicsContext.h"

namespace Owl
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& pProps);
		~WindowsWindow() override;

		void OnUpdate() override;

		unsigned int GetWidth() const override { return m_Data.Width; }
		unsigned int GetHeight() const override { return m_Data.Height; }

		void SetEventCallback(const EventCallbackFunction& pCallback) override { m_Data.EventCallback = pCallback; }
		void SetVSync(bool pEnabled) override;
		bool IsVSync() const override;

		void* GetNativeWindow() override { return m_Window; }

	protected:
		virtual void Init(const WindowProps& pProps);
		virtual void Shutdown();

	private:
		GLFWwindow* m_Window;

		Scope<GraphicsContext> m_Context;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool IsVSync;

			EventCallbackFunction EventCallback;
		};

		WindowData m_Data;
	};
}
