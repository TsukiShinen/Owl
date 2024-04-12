﻿#pragma once

#include "Owl/Core/Layer.h"

namespace Owl
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		virtual ~ImGuiLayer() = default;

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate() override;
		void OnEvent(Event& pEvent) override;
	private:
		float m_Time = 0.0f;
	};
	
}