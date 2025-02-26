#include "mopch.h"
#include "Layer.h"

#include "ModelOne/Core/Core.h"
#include "ModelOne/Events/Event.h"


namespace ModelOne {
	Layer::Layer(const std::string& name) : m_DebugName(name) {
	}

	Layer::~Layer() {
	}
}
