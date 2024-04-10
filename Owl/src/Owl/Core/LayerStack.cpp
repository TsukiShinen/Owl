#include "opch.h"
#include "LayerStack.h"

namespace Owl
{
	LayerStack::LayerStack()
	{
		m_LayersInsert = m_Layers.begin();
	}

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
		m_LayersInsert = m_Layers.emplace(m_LayersInsert, pLayer);
		pLayer->OnAttach();
	}

	void LayerStack::PushOverlay(Layer* pOverlay)
	{
		m_Layers.emplace_back(pOverlay);
		pOverlay->OnAttach();
	}

	void LayerStack::PopLayer(Layer* pLayer)
	{
		const auto iterator = std::ranges::find(m_Layers, pLayer);
		if (iterator != m_Layers.end())
		{
			pLayer->OnDetach();
			m_Layers.erase(iterator);
			--m_LayersInsert;
		}
	}

	void LayerStack::PopOverlay(Layer* pOverlay)
	{
		const auto iterator = std::ranges::find(m_Layers, pOverlay);
		if (iterator != m_Layers.end())
		{
			pOverlay->OnDetach();
			m_Layers.erase(iterator);
		}
	}
}
