#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Taller/Renderer/VertexArray.h"

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

		CameraComponent(float fov = 45.0f, float aspectRatio = 1.7f, float nearPlane = 0.1f, float farPlane = 100.0f) : projection(glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane)), viewProjection(glm::mat4(1.0f)) {
		}
	};

	struct StaticMeshComponent {
		std::string shaderName; // TODO Use material system!!!
		std::string textureName;// TODO Use material system!!!
		std::string meshName;

		StaticMeshComponent(const std::string& shaderName = "", const std::string& textureName = "", const std::string & meshName = "") : shaderName(shaderName), textureName(textureName), meshName(meshName) {
		}
	};

	struct AmbientLightComponent {
		glm::vec3 color;
		float intensity;

		AmbientLightComponent(glm::vec3 color = glm::vec3(1.0f), float intensity = 0.5f) : color(color), intensity(intensity) {
		}
	};

	struct DirectionalLightComponent {
		glm::vec3 color;
		glm::vec3 direction;
		float intensity;

		DirectionalLightComponent(glm::vec3 direction = glm::vec3(0.0f), glm::vec3 color = glm::vec3(1.0f), float intensity = 1.0f) : direction(direction), color(color), intensity(intensity) {
		}
	};






	enum class ForceGenerator {
		Gravity,
		Drag
	};

	struct PointMassComponent {
		glm::vec3 velocity;
		glm::vec3 acceleration;
		glm::vec3 forceAccumulation;

		float damping;
		float inverseMass;

		std::vector<ForceGenerator> influentialForces;

		PointMassComponent(glm::vec3 velocity = glm::vec3(0.0f), glm::vec3 acceleration = glm::vec3(0.0f), float damping = 0.9f, float inverseMass = 0.0f) : velocity(velocity), acceleration(acceleration), damping(damping), inverseMass(inverseMass), forceAccumulation({0}), influentialForces({ForceGenerator::Gravity, ForceGenerator::Drag}) {
		}
	};

	struct BoxCollisionComponent {
		glm::vec3 size;
		glm::vec3 minPoint;
		glm::vec3 maxPoint;

		std::function<void(int, int)> onCollisionFunction;

		BoxCollisionComponent(glm::vec3 size = { 0.05f, 0.05f, 0.05f }, std::function<void(int, int)> onCollisionFunction = [&](int a, int b){}) : size(size), onCollisionFunction(onCollisionFunction) {}
	};
}