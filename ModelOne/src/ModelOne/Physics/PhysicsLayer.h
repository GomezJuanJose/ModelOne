#pragma once
#include "ModelOne/Core/Layer.h"
#include "ModelOne/Core/Core.h"

#include "ModelOne/Core/Timestep.h"
#include "ModelOne/ECS/Components.h"
#include "ModelOne/ECS/ECS.h"

#include "ModelOne/Core/Application.h"

namespace ModelOne {
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
