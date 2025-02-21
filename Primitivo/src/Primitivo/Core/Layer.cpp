#include "plpch.h"
#include "Layer.h"

#include "Primitivo/Core/Core.h"
#include "Primitivo/Events/Event.h"


namespace Primitivo {
	Layer::Layer(const std::string& name) : m_DebugName(name) {
	}

	Layer::~Layer() {
	}
}
