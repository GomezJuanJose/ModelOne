#include <ModelOne.h>
#include <ModelOne/Core/EntryPoint.h>
#include <ModelOne/Core/Window.h>
#include <ModelOne/Core/KeyCodes.h>
#include <Platform/OpenGL/OpenGLShader.h>
#include <ModelOne/Events/KeyEvents.h>
#include <ModelOne/Events/MouseEvents.h>
#include "ImGui/imgui.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include "GameComponents.h"

#include "GameComponentsOperations.h"


#define MO_MOUSE_KEY -1


class GameLogicLayer : public ModelOne::Layer {
public:
	GameLogicLayer() : Layer("GameLogicLayer") {

	};

	virtual void OnUpdate(ModelOne::Timestep timestep) {
		std::set<ModelOne::Entity> entities = coord.GetEntitiesByGroup("Player");
		for each (ModelOne::Entity ent in entities) {
			ModelOne::TransformComponent& transform = coord.GetComponent<ModelOne::TransformComponent>(ent);
			SandboxApp::PlayerMovement& player_movement = coord.GetComponent<SandboxApp::PlayerMovement>(ent);
			transform.location.x -= (player_movement.left_current_speed + player_movement.right_current_speed) * timestep.GetSeconds();
			transform.location.z -= (player_movement.forward_current_speed + player_movement.backward_current_speed) * timestep.GetSeconds();

			transform.rotation.y += player_movement.x_axis_speed * timestep.GetSeconds();
			player_movement.x_axis_speed = 0;
			transform.rotation.x += player_movement.y_axis_speed * timestep.GetSeconds();
			player_movement.y_axis_speed = 0;
		}
	}

	virtual void OnEvent(ModelOne::Event& e) {
		if (e.IsInCategory(ModelOne::EventCategoryKeyboard)) {
			int keycode = static_cast<ModelOne::KeyEvent&>(e).GetKeyCode();

			std::set<ModelOne::Entity> entities = coord.GetEntitiesByGroup("Player");

			for each (ModelOne::Entity ent in entities) {
				// TODO: LIMPIARLO
				if (coord.GetComponent<SandboxApp::InputComponent>(ent).keys_functions.find(keycode) != coord.GetComponent<SandboxApp::InputComponent>(ent).keys_functions.end()) {
					coord.GetComponent<SandboxApp::InputComponent>(ent).keys_functions.at(keycode)(coord.GetComponent<SandboxApp::PlayerMovement>(ent), keycode, &e);
				}
			}
		} 
		else if (e.IsInCategory(ModelOne::EventCategoryMouse)) {
			std::set<ModelOne::Entity> entities = coord.GetEntitiesByGroup("Player");

			for each (ModelOne::Entity ent in entities) {
				coord.GetComponent<SandboxApp::InputComponent>(ent).keys_functions.at(MO_MOUSE_KEY)(coord.GetComponent<SandboxApp::PlayerMovement>(ent), MO_MOUSE_KEY, &e);
			}
		}
	};

private:

	ModelOne::Coordinator& coord = ModelOne::Application::Get().GetCoordinator();
};


class EntittiesLayer : public ModelOne::Layer {
public:
	EntittiesLayer() : Layer("EntittiesLayer") {

		//Creates ambient and directional light
		ModelOne::Entity light = coord.CreateEntity();
		coord.AddComponent<ModelOne::AmbientLightComponent>(light, glm::vec3(1.0f), 0.8f);
		coord.AddComponent<ModelOne::DirectionalLightComponent>(light, glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec3(1.0f), 0.3f);
		coord.GroupEntity(light, "generalLight");




		// Create axolotl
		ModelOne::Entity axolotl = coord.CreateEntity();
		coord.AddComponent<ModelOne::TransformComponent>(axolotl, glm::vec3(0.0f), glm::vec3(0.0f, -45.0f, 0.0f), glm::vec3(0.35f));
		coord.AddComponent<ModelOne::StaticMeshComponent>(axolotl, "object", "axolotl_cyan", "axolotl");
		coord.AddComponent<ModelOne::PointMassComponent>(axolotl, glm::vec3(0.0f), glm::vec3(0.0f), 1.0f, ((float)1.0f) / 20.0f);
		coord.AddComponent<ModelOne::BoxCollisionComponent>(axolotl, glm::vec3(0.05f, 0.05f, 0.05f), [&](int a, int b) { ModelOne::AudioCommand::PlayAudio(ModelOne::Application::Get().GetAudioLibrary().Get("sfx"), false, 1.0f, 0.0f); });
		ModelOne::StaticMeshComponent& axolotl_SM = coord.GetComponent<ModelOne::StaticMeshComponent>(axolotl);
		coord.GroupEntity(axolotl, "Movable");

		ModelOne::Application::Get().GetTextureLibrary().Load(axolotl_SM.textureName, "assets/textures/axolotl_cyan.png");
		ModelOne::Application::Get().GetMeshLibrary().Load(axolotl_SM.meshName, "assets/3dmodels/untitled.obj");
		//----------------------------------//

		// Create axolotl
		ModelOne::Entity axolotl_gold = coord.CreateEntity();
		coord.AddComponent<ModelOne::TransformComponent>(axolotl_gold, glm::vec3(0.0f, -1.5f, 0.0f), glm::vec3(0.0f), glm::vec3(0.35f));
		coord.AddComponent<ModelOne::StaticMeshComponent>(axolotl_gold, "object", "axolotl_gold", "axolotl");
		coord.AddComponent<ModelOne::PointMassComponent>(axolotl_gold, glm::vec3(0.0f), glm::vec3(0.0f), 0.8f, ((float)1.0f) / 20.0f);
		coord.AddComponent<ModelOne::BoxCollisionComponent>(axolotl_gold, glm::vec3(0.05f, 0.05f, 0.05f), [&](int a, int b) { ModelOne::AudioCommand::PlayAudio(ModelOne::Application::Get().GetAudioLibrary().Get("sfx"), false, 1.0f, 0.0f); });
		ModelOne::StaticMeshComponent& axolotl_gold_SM = coord.GetComponent<ModelOne::StaticMeshComponent>(axolotl_gold);
		coord.GroupEntity(axolotl_gold, "Movable");

		ModelOne::Application::Get().GetTextureLibrary().Load(axolotl_gold_SM.textureName, "assets/textures/axolotl_gold.png");
		//----------------------------------//


		/* CREATES A GROUND PLANE ENTITY */
		ModelOne::Entity plane = coord.CreateEntity();
		coord.AddComponent<ModelOne::TransformComponent>(plane, glm::vec3(0.0f, -3.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(15.0f, 0.1f, 15.0f));
		coord.AddComponent<ModelOne::StaticMeshComponent>(plane, "CubeShader", "", "Cube");
		coord.AddComponent<ModelOne::BoxCollisionComponent>(plane, glm::vec3({ 1000.0f, 0.1f, 1000.0f }), [&](int a, int b) {});
		ModelOne::StaticMeshComponent& plane_SM = coord.GetComponent<ModelOne::StaticMeshComponent>(plane);
		coord.GroupEntity(plane, "Static");
		ModelOne::Application::Get().GetMeshLibrary().Load(plane_SM.meshName, "assets/3dmodels/cube.obj");
		ModelOne::Application::Get().GetShaderLibrary().Load(plane_SM.shaderName, "assets/shaders/cube.glsl");
		//----------------------------------//





		/* CREATES THE CAMERA */
		ModelOne::Entity player = coord.CreateEntity();
		coord.AddComponent<ModelOne::CameraComponent>(player, 45.0f, 16.0f / 9.0f, 0.1f, 100.0f);
		coord.AddComponent<ModelOne::TransformComponent>(player);
		//TODO: Limpiarlo
		using func = void(*)(SandboxApp::PlayerMovement& player_mov, int keycode, ModelOne::Event* e);
		std::unordered_map<int, func> keys_functions = { {MO_KEY_A, SandboxApp::MovePlayer}, {MO_KEY_D, SandboxApp::MovePlayer}, {MO_KEY_W, SandboxApp::MovePlayer}, {MO_KEY_S, SandboxApp::MovePlayer}, {MO_MOUSE_KEY, SandboxApp::RotatePlayer} };
		coord.AddComponent<SandboxApp::InputComponent>(player, keys_functions);
		coord.AddComponent<SandboxApp::PlayerMovement>(player);

		coord.GroupEntity(player, "camera");
		coord.GroupEntity(player, "Player");

		ModelOne::TransformComponent& transformCam = coord.GetComponent<ModelOne::TransformComponent>(player);
		transformCam.location = glm::vec3(0.0f, 0.0f, -10.0f);
		transformCam.rotation = glm::vec3(0.0f, 0.0f, 0.0f);

	};

private:

	ModelOne::Coordinator& coord = ModelOne::Application::Get().GetCoordinator();
};



class AudioLoaderLayer : public ModelOne::Layer {
public:
	AudioLoaderLayer() : Layer("AudioLayer") {
		ModelOne::Application::Get().GetAudioLibrary().Load("sfx", "C:\\dev\\GameEngine_ModelOne\\ModelOne\\SandboxProjects\\Sandbox\\assets\\audios\\boing.wav");
		ModelOne::Application::Get().GetAudioLibrary().Load("music", "C:\\dev\\GameEngine_ModelOne\\ModelOne\\SandboxProjects\\Sandbox\\assets\\audios\\music.wav");

		ModelOne::AudioCommand::PlayAudio(ModelOne::Application::Get().GetAudioLibrary().Get("music"), true, 1.0f, 0.0f);

	};

private:
	ModelOne::Coordinator& coord = ModelOne::Application::Get().GetCoordinator();

	ModelOne::Signature audioSignature;
};






class Sandbox : public ModelOne::Application {
public:
	Sandbox() {
		PushLayer(new EntittiesLayer());
		PushLayer(new AudioLoaderLayer());
		PushLayer(new GameLogicLayer());
	}
	~Sandbox() {}

};

ModelOne::Application* ModelOne::CreateApplication() {
	return new Sandbox();
}

