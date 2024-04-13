#include "opch.h"
#include "LayerStack.h"

namespace Owl
{
	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_Layers)
		{
			layer->OnDetach();
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* pLayer)
	{
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, pLayer);
		m_LayerInsertIndex++;
		pLayer->OnAttach();
	}

	void LayerStack::PushOverlay(Layer* pOverlay)
	{
		m_Layers.emplace_back(pOverlay);
		pOverlay->OnAttach();
	}

	void LayerStack::PopLayer(Layer* pLayer)
	{
		const auto iterator = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex, pLayer);
		if (iterator != m_Layers.begin() + m_LayerInsertIndex)
		{
			pLayer->OnDetach();
			m_Layers.erase(iterator);
			m_LayerInsertIndex--;
		}
	}

	void LayerStack::PopOverlay(Layer* pOverlay)
	{
		const auto iterator = std::find(m_Layers.begin() + m_LayerInsertIndex, m_Layers.end(), pOverlay);
		if (iterator != m_Layers.end())
		{
			pOverlay->OnDetach();
			m_Layers.erase(iterator);
		}
	}
}
