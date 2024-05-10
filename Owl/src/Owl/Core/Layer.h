#pragma once
#include "DeltaTime.h"
#include "Owl/Events/Event.h"

namespace Owl
{
	class Layer
	{
	public:
		Layer(std::string pName = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach()
		{
		}

		virtual void OnDetach()
		{
		}

		virtual void OnUpdate(DeltaTime pDeltaTime)
		{
		}

		virtual void OnImGuiRender()
		{
		}

		virtual void OnEvent(Event& pEvent)
		{
		}

		const std::string& GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;
	};
}
