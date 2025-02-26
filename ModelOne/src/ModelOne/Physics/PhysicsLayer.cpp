#include "mopch.h"
#include "PhysicsLayer.h"


#include "ModelOne/FunctionsLibraries/ComponentOperations.h"

namespace ModelOne {
	PhysicsLayer::PhysicsLayer() : Layer("PhysicsLayer") {

		coord.AddQueryRequirement<TransformComponent>();
		coord.AddQueryRequirement<PointMassComponent>();
		physicsSignature = coord.RegisterQueryRequirement();

		coord.AddQueryRequirement<TransformComponent>();
		coord.AddQueryRequirement<BoxCollisionComponent>();
		collisionSignature = coord.RegisterQueryRequirement();


		forcesRegistry[ForceGenerator::Gravity] = &ComponentOperations::ApplyGravityForce;
		forcesRegistry[ForceGenerator::Drag] = &ComponentOperations::ApplyDragForce;

	};

	void PhysicsLayer::OnUpdate(Timestep timestep) {
		MO_PROFILE_FUNCTION();

		//Applies forces
		{
			MO_PROFILE_SCOPE("Apply forces");

			for (Entity entity : coord.QueryEntitiesBySignature(physicsSignature)) {

				PointMassComponent& pointMass = coord.GetComponent<PointMassComponent>(entity.GetId());

				//Apply all forces before the integration
				for (ForceGenerator influentialForce : pointMass.influentialForces) {
					forcesRegistry[influentialForce](pointMass, timestep);
				}

			}
		}


		//Collision detection
		{
			MO_PROFILE_SCOPE("Collision detection and resolution");

			std::set<Entity> entities = coord.QueryEntitiesBySignature(collisionSignature);
			for (auto i = entities.begin(); i != entities.end(); i++) {
				Entity entity = *i;
				TransformComponent& transform = coord.GetComponent<TransformComponent>(entity.GetId());
				BoxCollisionComponent& boxCollision = coord.GetComponent<BoxCollisionComponent>(entity.GetId());

				if (coord.HasComponent < PointMassComponent>(entity)) {
					PointMassComponent& pointmass = coord.GetComponent<PointMassComponent>(entity.GetId());
					debugMaxCollisionPoint = pointmass.velocity;
				}
				boxCollision.maxPoint = transform.location + boxCollision.size;
				boxCollision.minPoint = transform.location - boxCollision.size;




				for (auto j = entities.begin(); j != entities.end(); j++) {
					Entity auxiliarEntity = *j;

					if (auxiliarEntity.GetId() == entity.GetId()) {
						continue;
					}

					TransformComponent& transformAuxiliar = coord.GetComponent<TransformComponent>(auxiliarEntity.GetId());
					BoxCollisionComponent& boxCollisionAuxiliar = coord.GetComponent<BoxCollisionComponent>(auxiliarEntity.GetId());

					if (ComponentOperations::TestAABBToAABB(boxCollision, boxCollisionAuxiliar)) {
						MO_LOG_INFO(true, "Collision success : %s with %s", std::to_string(entity.GetId()), std::to_string(auxiliarEntity.GetId()));

						boxCollision.onCollisionFunction(entity.GetId(), auxiliarEntity.GetId());
						boxCollisionAuxiliar.onCollisionFunction(entity.GetId(), auxiliarEntity.GetId());

						//Collision resolution					
						bool hasAMass = coord.HasComponent<PointMassComponent>(entity.GetId());
						bool hasBMass = coord.HasComponent<PointMassComponent>(auxiliarEntity.GetId());
						//Apply a general resolution by force for resting bodies, moving bodies and interpenetration. In the future use a specific formula to resolve each case, collision, interprenetation and resting contact


						if (!hasAMass) {
							PointMassComponent& pointMassB = coord.GetComponent<PointMassComponent>(auxiliarEntity.GetId());
							ComponentOperations::BasicCollisionResolution(transform, PointMassComponent(), transformAuxiliar, pointMassB, timestep);
						}
						else if (!hasBMass) {
							PointMassComponent& pointMassA = coord.GetComponent<PointMassComponent>(entity.GetId());
							ComponentOperations::BasicCollisionResolution(transform, pointMassA, transformAuxiliar, PointMassComponent(), timestep);
						}
						else {
							PointMassComponent& pointMassA = coord.GetComponent<PointMassComponent>(entity.GetId());
							PointMassComponent& pointMassB = coord.GetComponent<PointMassComponent>(auxiliarEntity.GetId());
							ComponentOperations::BasicCollisionResolution(transform, pointMassA, transformAuxiliar, pointMassB, timestep);
						}

					}
				}
			}
		}

		//Applies forces
		{
			MO_PROFILE_SCOPE("Integrate");

			for (Entity entity : coord.QueryEntitiesBySignature(physicsSignature)) {
				TransformComponent& transform = coord.GetComponent<TransformComponent>(entity.GetId());
				PointMassComponent& pointMass = coord.GetComponent<PointMassComponent>(entity.GetId());

				ComponentOperations::Integrate(transform, pointMass, timestep);
			}
		}
	}
}