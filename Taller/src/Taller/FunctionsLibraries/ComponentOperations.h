#pragma once

#include "Taller/ECS/Components.h"

//TODO try to make all those operations more generic for any data struct

namespace Taller {
	class ComponentOperations {
	public:
		static glm::mat4 CalculateCameraViewProjection(CameraComponent& camera, TransformComponent& transform) {

			TL_PROFILE_FUNCTION();

			/* TODO This is not efficient and introduce the Gimbal locks, change it to quaternions*/
			return camera.projection * glm::inverse(glm::translate(glm::mat4(1.0f), transform.location)
				* glm::rotate(glm::mat4(1.0), glm::radians(transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f))
				* glm::rotate(glm::mat4(1.0), glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f))
				* glm::rotate(glm::mat4(1.0), glm::radians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f))
				* glm::scale(glm::mat4(1.0f), transform.scale));
		}

		static void Integrate(TransformComponent& transform, PointMassComponent& pointMass, float timestep) {
			
			TL_PROFILE_FUNCTION();

			if (pointMass.inverseMass <= 0.0f) {
				return;
			}

			transform.location += pointMass.velocity * timestep;
			
			glm::vec3 resultingAcc = pointMass.acceleration;
			resultingAcc += pointMass.forceAccumulation * pointMass.inverseMass;

			pointMass.velocity += resultingAcc * timestep;
			pointMass.velocity *= glm::pow(pointMass.damping, timestep);

			pointMass.forceAccumulation = { 0 }; //Clears all the force accumulation
		}

		static void ApplyGravityForce(PointMassComponent& pointMass, float timestep) {
			if (pointMass.inverseMass <= 0) {
				return;
			}

			glm::vec3 gravity = { 0.0f, -1.0f, 0.0f };

			pointMass.forceAccumulation += gravity * (1.0f/pointMass.inverseMass);
		}

		static void ApplyDragForce(PointMassComponent& pointMass, float timestep) {
			
			float k1 = 0.9f;
			float k2 = 0.5f;
			
			if (glm::length(pointMass.velocity) == 0.0f) {
				return;
			}

			glm::vec3 force = pointMass.velocity;
			float dragCoefficient = glm::length(force);
			dragCoefficient = k1 * dragCoefficient + k2 * dragCoefficient * dragCoefficient;

			force = glm::normalize(force);
			force *= -dragCoefficient;

			pointMass.forceAccumulation += force;
		}
	};
}