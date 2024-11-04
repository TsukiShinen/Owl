#pragma once

#include "KeyCodes.h"
#include "MouseCodes.h"
#include "Owl/Core/Base.h"

namespace Owl
{
	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode pKeyCode);

		static bool IsMouseButtonPressed(MouseCode pMouseCode);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}
