#pragma once
#include "Owl/Core/Input.h"

namespace Owl
{
	class WindowsInput : public Input
	{
	public:
		~WindowsInput() override = default;

	protected:
		bool IsKeyPressedImpl(KeyCode pKeyCode) override;
		bool IsMouseButtonPressedImpl(MouseCode pMouseCode) override;
		float GetCursorXImpl() override;
		float GetCursorYImpl() override;
		std::pair<float, float> GetCursorPositionImpl() override;
	};
}
