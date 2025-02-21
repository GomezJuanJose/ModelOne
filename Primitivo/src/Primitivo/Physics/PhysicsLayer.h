#pragma once
#include "Primitivo/Core/Layer.h"
#include "Primitivo/Core/Core.h"

#include "Primitivo/Core/Timestep.h"
#include "Primitivo/ECS/Components.h"
#include "Primitivo/ECS/ECS.h"

#include "Primitivo/Core/Application.h"

namespace Primitivo {
	class PhysicsLayer : public Layer {
	public:
		PhysicsLayer();
		~PhysicsLayer() = default;


		virtual void OnUpdate(Timestep timestep) override;

	private:
		Coordinator& coord = Application::Get().GetCoordinator();

		Signature physicsSignature;
		Signature collisionSignature;


		using forceFunction = std::function<void(PointMassComponent&, float)>;
		std::unordered_map<ForceGenerator, forceFunction> forcesRegistry;

		glm::vec3 debugMaxCollisionPoint;
		glm::vec3 debugMinCollisionPoint;
	};
}
