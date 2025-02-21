#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Primitivo/Renderer/VertexArray.h"

namespace Primitivo {
	struct TransformComponent {
		glm::vec3 location;
		glm::vec3 rotation;
		glm::vec3 scale;

		bool is_valid;

		TransformComponent(glm::vec3 location = glm::vec3(0.0f), glm::vec3 rotation = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f)) : location(location), rotation(rotation), scale(scale), is_valid(true) {
		}
	};

	struct CameraComponent {
		glm::mat4 projection;
		glm::mat4 viewProjection;

		bool is_valid;

		CameraComponent(float fov = 45.0f, float aspectRatio = 1.7f, float nearPlane = 0.1f, float farPlane = 100.0f) : projection(glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane)), viewProjection(glm::mat4(1.0f)), is_valid(true) {
		}
	};

	struct StaticMeshComponent {
		std::string shaderName; // TODO material system
		std::string textureName;// TODO material system
		std::string meshName;

		bool is_valid;

		StaticMeshComponent(const std::string& shaderName = "", const std::string& textureName = "", const std::string & meshName = "") : shaderName(shaderName), textureName(textureName), meshName(meshName), is_valid(true) {
		}
	};

	struct AmbientLightComponent {
		glm::vec3 color;
		float intensity;

		bool is_valid;

		AmbientLightComponent(glm::vec3 color = glm::vec3(1.0f), float intensity = 0.5f) : color(color), intensity(intensity), is_valid(true) {
		}
	};

	struct DirectionalLightComponent {
		glm::vec3 color;
		glm::vec3 direction;
		float intensity;

		bool is_valid;

		DirectionalLightComponent(glm::vec3 direction = glm::vec3(0.0f), glm::vec3 color = glm::vec3(1.0f), float intensity = 1.0f) : direction(direction), color(color), intensity(intensity), is_valid(true) {
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

		bool is_valid;

		PointMassComponent(glm::vec3 velocity = glm::vec3(0.0f), glm::vec3 acceleration = glm::vec3(0.0f), float damping = 0.9f, float inverseMass = 0.0f) : velocity(velocity), acceleration(acceleration), damping(damping), inverseMass(inverseMass), forceAccumulation({0}), influentialForces({ForceGenerator::Gravity, ForceGenerator::Drag}), is_valid(true) {
		}
	};

	struct BoxCollisionComponent {
		glm::vec3 size;
		glm::vec3 minPoint;
		glm::vec3 maxPoint;

		std::function<void(int, int)> onCollisionFunction;

		bool is_valid;

		BoxCollisionComponent(glm::vec3 size = { 0.05f, 0.05f, 0.05f }, std::function<void(int, int)> onCollisionFunction = [&](int a, int b){}) : size(size), onCollisionFunction(onCollisionFunction), is_valid(true) {}
	};
}