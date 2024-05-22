#pragma once

#include "Components.h"

namespace Taller {
	class ComponentOperations {
	public:
		static void CalculateCameraViewProjection(CameraComponent& camera, TransformComponent& transform) {
			/* TODO This is not efficient and introduce the Gimbal locks, change it to quaternions*/
			camera.viewProjection = camera.projection * glm::inverse(glm::translate(glm::mat4(1.0f), transform.location)
				* glm::rotate(glm::mat4(1.0), glm::radians(transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f))
				* glm::rotate(glm::mat4(1.0), glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f))
				* glm::rotate(glm::mat4(1.0), glm::radians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f))
				* glm::scale(glm::mat4(1.0f), transform.scale));
		}
	};
}