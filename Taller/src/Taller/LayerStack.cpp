#include "tlpch.h"
#include "LayerStack.h"


namespace Taller {
	Taller::LayerStack::LayerStack() {
		m_LayerInsert = m_Layers.begin();
	}

	Taller::LayerStack::~LayerStack() {
		for (Layer* layer : m_Layers) {
			delete layer;
		}
	}

	void Taller::LayerStack::PushLayer(Layer* layer) {
		m_LayerInsert = m_Layers.emplace(m_LayerInsert, layer);
	}

	void Taller::LayerStack::PushOverlay(Layer* overlay) {
		m_Layers.emplace_back(overlay);
	}

	void Taller::LayerStack::PopLayer(Layer* layer) {
		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
		if (it != m_Layers.end()) {
			m_Layers.erase(it);
			m_LayerInsert--;
		}
	}

	void Taller::LayerStack::PopOverlay(Layer* overlay) {
		auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
		if (it != m_Layers.end()) {
			m_Layers.erase(it);
		}
	}
}