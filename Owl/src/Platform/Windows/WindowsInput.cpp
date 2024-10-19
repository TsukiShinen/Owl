#include "opch.h"
#include "Owl/Core/Input.h"

#include <GLFW/glfw3.h>

#include "Owl/Core/Application.h"

namespace Owl
{
	bool Input::IsKeyPressed(const KeyCode pKeyCode)
	{
		const Application& app = Application::Get();
		const auto window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		const auto state = glfwGetKey(window, pKeyCode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonPressed(const MouseCode pMouseCode)
	{
		const Application& app = Application::Get();
		const auto window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		const auto state = glfwGetMouseButton(window, pMouseCode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	std::pair<float, float> Input::GetMousePosition()
	{
		const Application& app = Application::Get();
		const auto window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		double x, y;
		glfwGetCursorPos(window, &x, &y);

		return {static_cast<float>(x), static_cast<float>(y)};
	}

	float Input::GetMouseX()
	{
		auto [x, y] = GetMousePosition();

		return x;
	}

	float Input::GetMouseY()
	{
		auto [x, y] = GetMousePosition();

		return y;
	}
}
