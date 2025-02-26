#pragma once

#include "ModelOne/ECS/Components.h"

//TODO try to make all those operations more generic for any data struct

namespace ModelOne {
	class ComponentOperations {
	public:
		static glm::mat4 CalculateCameraViewProjection(CameraComponent& camera, TransformComponent& transform) {

			MO_PROFILE_FUNCTION();

			/* TODO This is not efficient and introduce the Gimbal locks, change it to quaternions*/
			return camera.projection * glm::translate(glm::mat4(1.0f), transform.location)
				* glm::rotate(glm::mat4(1.0), glm::radians(transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f))
				* glm::rotate(glm::mat4(1.0), glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f))
				* glm::rotate(glm::mat4(1.0), glm::radians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f))
				* glm::scale(glm::mat4(1.0f), transform.scale);
		}

		static void Integrate(TransformComponent& transform, PointMassComponent& pointMass, float timestep) {
			
			MO_PROFILE_FUNCTION();

			if (pointMass.inverseMass <= 0.0f) {
				return;
			}

			transform.location += pointMass.velocity * timestep;
			
			glm::vec3 resultingAcc = pointMass.acceleration;
			resultingAcc += pointMass.forceAccumulation * pointMass.inverseMass;

			pointMass.velocity += resultingAcc * timestep;
			pointMass.velocity *= glm::pow(pointMass.damping, timestep);

			pointMass.forceAccumulation = glm::vec3(0.0f); //Clears all the force accumulation
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

		static bool TestAABBToAABB(BoxCollisionComponent& box1, BoxCollisionComponent& box2) {
			bool result = box1.maxPoint.x < box2.minPoint.x ||
				box1.maxPoint.y < box2.minPoint.y ||
				box1.maxPoint.z < box2.minPoint.z ||
				box2.maxPoint.x < box1.minPoint.x ||
				box2.maxPoint.y < box1.minPoint.y ||
				box2.maxPoint.z < box1.minPoint.z;

			return !result;
		}

		static void BasicCollisionResolution(TransformComponent& transformA, PointMassComponent& pointMassA, TransformComponent& transformB, PointMassComponent& pointMassB, float timestep) {
			//Calculate normal
			glm::vec3 normalImpact = transformA.location - transformB.location;
			float m = glm::sqrt(normalImpact.x * normalImpact.x + normalImpact.y * normalImpact.y + normalImpact.z * normalImpact.z);
			normalImpact.x /= m;
			normalImpact.y /= m;
			normalImpact.z /= m;

			float restitution = 1.0f;

			//Find the velocity in the direction of the contact
			float separatingVelocity;
			glm::vec3 relativeVelocity;
			relativeVelocity = pointMassA.velocity;
			relativeVelocity -= pointMassB.velocity;

			separatingVelocity = (relativeVelocity.x * normalImpact.x) + (relativeVelocity.y * normalImpact.y) + (relativeVelocity.z * normalImpact.z);

			//Check if it needs to be resolved
			if (separatingVelocity > 0) {
				//The contact is either separating, or stationary, no need to impulse
				return;
			}

			//Calculate the new separating velocity
			float newSeparatingVelocity = -separatingVelocity * restitution;

			glm::vec3 accumulatedCausedVelocity;
			accumulatedCausedVelocity = pointMassA.acceleration;
			accumulatedCausedVelocity -= pointMassB.acceleration;


			float accumulatedCausedSeparationVelocity = ((accumulatedCausedVelocity.x * normalImpact.x) + (accumulatedCausedVelocity.y * normalImpact.y) + (accumulatedCausedVelocity.z * normalImpact.z)) * timestep;

			if (accumulatedCausedSeparationVelocity < 0.0f) {
				newSeparatingVelocity += restitution * accumulatedCausedSeparationVelocity;

				if (newSeparatingVelocity < 0.0f) {
					newSeparatingVelocity = 0.0f;
				}
			}

			float deltaVelocity = newSeparatingVelocity - separatingVelocity;

			float totalInverseMass = 0;
			totalInverseMass = pointMassA.inverseMass;
			totalInverseMass += pointMassB.inverseMass;


			if (totalInverseMass <= 0) {
				return;
			}

			float impulse = deltaVelocity / totalInverseMass;

			glm::vec3 impulsePerInverseMass = normalImpact * impulse;

			pointMassA.velocity = pointMassA.velocity + impulsePerInverseMass * pointMassA.inverseMass;
			pointMassB.velocity = pointMassB.velocity + impulsePerInverseMass * -pointMassB.inverseMass;
		}
	};
}