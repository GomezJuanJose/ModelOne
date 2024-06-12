#pragma once

#include "Taller/ECS/Components.h"

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
	};
}