#pragma once
#include "Layer.h"

namespace Owl
{
	class LayerStack
	{
	public:
		LayerStack() = default;
		~LayerStack();

		void PushLayer(Layer* pLayer);
		void PushOverlay(Layer* pOverlay);
		void PopLayer(Layer* pLayer);
		void PopOverlay(Layer* pOverlay);

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }
	private:
		std::vector<Layer*> m_Layers;
		unsigned int m_LayerInsertIndex = 0;
	};
}
