﻿#pragma once
#include "Owl/Events/Event.h"

namespace Owl
{
	class Layer
	{
	public:
		Layer(const std::string& pName = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnEvent(Event& pEvent) {}

		const std::string& GetName() const { return m_DebugName; }
		
	protected:
		std::string m_DebugName;
	};
}
