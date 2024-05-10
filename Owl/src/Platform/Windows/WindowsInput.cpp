#include "opch.h"
#include "WindowsInput.h"

#include "GLFW/glfw3.h"
#include "Owl/Core/Application.h"

namespace Owl
{
	Scope<Input> Input::s_Instance = CreateScope<WindowsInput>();

	bool WindowsInput::IsKeyPressedImpl(const KeyCode pKeyCode)
	{
		const Application& app = Application::Get();
		const auto window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		const auto state = glfwGetKey(window, pKeyCode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool WindowsInput::IsMouseButtonPressedImpl(const MouseCode pMouseCode)
	{
		const Application& app = Application::Get();
		const auto window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		const auto state = glfwGetMouseButton(window, pMouseCode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	float WindowsInput::GetCursorXImpl()
	{
		auto [x, y] = GetCursorPositionImpl();

		return x;
	}

	float WindowsInput::GetCursorYImpl()
	{
		auto [x, y] = GetCursorPositionImpl();

		return y;
	}

	std::pair<float, float> WindowsInput::GetCursorPositionImpl()
	{
		const Application& app = Application::Get();
		const auto window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		double x, y;
		glfwGetCursorPos(window, &x, &y);

		return {static_cast<float>(x), static_cast<float>(y)};
	}
}
