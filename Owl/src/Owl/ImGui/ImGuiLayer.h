#pragma once

#include "Owl/Core/Layer.h"

namespace Owl
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() override = default;

		void OnAttach() override;
		void OnDetach() override;
		void OnEvent(Event& pEvent) override;

		void Begin();
		void End();

		void BlockEvents(bool pBlock) { m_BlockEvents = pBlock; }
	private:
		bool m_BlockEvents = true;
	};
}
