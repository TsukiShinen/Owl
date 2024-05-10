#pragma once

#include "KeyCodes.h"
#include "MouseCodes.h"
#include "Owl/Core/Base.h"

namespace Owl
{
	class Input
	{
	public:
		virtual ~Input() = default;

		Input(const Input&) = delete;
		Input& operator=(const Input&) = delete;

		static bool IsKeyPressed(const KeyCode pKeyCode) { return s_Instance->IsKeyPressedImpl(pKeyCode); }

		static bool IsMouseButtonPressed(const MouseCode pMouseCode)
		{
			return s_Instance->IsMouseButtonPressedImpl(pMouseCode);
		}

		static std::pair<float, float> GetCursorPosition() { return s_Instance->GetCursorPositionImpl(); }
		static float GetCursorX() { return s_Instance->GetCursorXImpl(); }
		static float GetCursorY() { return s_Instance->GetCursorYImpl(); }

	protected:
		Input() = default;

		virtual bool IsKeyPressedImpl(KeyCode pKeyCode) = 0;

		virtual bool IsMouseButtonPressedImpl(MouseCode pMouseCode) = 0;
		virtual std::pair<float, float> GetCursorPositionImpl() = 0;
		virtual float GetCursorXImpl() = 0;
		virtual float GetCursorYImpl() = 0;

	private:
		static Scope<Input> s_Instance;
	};
}
