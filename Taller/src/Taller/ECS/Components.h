#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Taller {
	struct TransformComponent {
		glm::vec3 location;
		glm::vec3 rotation;
		glm::vec3 scale;

		TransformComponent(glm::vec3 location = glm::vec3(0.0f), glm::vec3 rotation = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f)) : location(location), rotation(rotation), scale(scale) {
		}
	};

	struct CameraComponent {
		glm::mat4 projection;
		glm::mat4 viewProjection;

		CameraComponent(float fov = 45.0f, float aspectRatio = 1.7f, float nearPlane = 0.1f, float farPlane = 100.0f) : projection(glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane)) {
		}
	};
}