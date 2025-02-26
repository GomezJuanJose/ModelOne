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
			transform.location.y -= (player_movement.up_current_speed + player_movement.down_current_speed) * timestep.GetSeconds();

			if (player_movement.enable_camera_movement) {
				transform.rotation.y += player_movement.x_axis_speed * timestep.GetSeconds();
				player_movement.x_axis_speed = 0;
				transform.rotation.x += player_movement.y_axis_speed * timestep.GetSeconds();
				player_movement.y_axis_speed = 0;
			}
		}
	}

	virtual void OnEvent(ModelOne::Event& e) {
		if (e.IsInCategory(ModelOne::EventCategoryKeyboard)) {
			int keycode = static_cast<ModelOne::KeyEvent&>(e).GetKeyCode();

			std::set<ModelOne::Entity> entities = coord.GetEntitiesByGroup("Player");

			for each (ModelOne::Entity ent in entities) {
				std::unordered_map<int, SandboxApp::input_func>& keys_functions = coord.GetComponent<SandboxApp::InputComponent>(ent).keys_functions;
				if (keys_functions.find(keycode) != keys_functions.end()) {
					keys_functions.at(keycode)(coord.GetComponent<SandboxApp::PlayerMovement>(ent), keycode, &e);
				}
			}
		}
		else if (e.IsInCategory(ModelOne::EventCategoryMouse)) {
			std::set<ModelOne::Entity> entities = coord.GetEntitiesByGroup("Player");

			for each (ModelOne::Entity ent in entities) {
				std::unordered_map<int, SandboxApp::input_func>& keys_functions = coord.GetComponent<SandboxApp::InputComponent>(ent).keys_functions;
				keys_functions.at(MO_MOUSE_KEY)(coord.GetComponent<SandboxApp::PlayerMovement>(ent), MO_MOUSE_KEY, &e);
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




		for (int i = 0; i < 5; i++) {
			ModelOne::Application::Get().GetTextureLibrary().Load(texture_names[i], "assets/textures/" + texture_names[i] + ".png");
		}
		ModelOne::Application::Get().GetMeshLibrary().Load("axolotl_mesh", "assets/3dmodels/untitled.obj");

		int number_axolotl = 20;
		for (int i = 0; i < number_axolotl; i++) {
			SpawnAxolotl();
		}


		/* CREATES A GROUND PLANE ENTITY */
		ModelOne::Entity plane = coord.CreateEntity();
		coord.AddComponent<ModelOne::TransformComponent>(plane, glm::vec3(0.0f, -3.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(15.0f, 0.1f, 15.0f));
		coord.AddComponent<ModelOne::StaticMeshComponent>(plane, "object", "paving", "Cube");
		coord.AddComponent<ModelOne::BoxCollisionComponent>(plane, glm::vec3({ 100000.0f, 0.05f, 100000.0f }), [&](int a, int b) {});
		//coord.AddComponent<ModelOne::PointMassComponent>(plane, glm::vec3(0.0f), glm::vec3(0.0f), 0.0f, 0.0f);
		ModelOne::StaticMeshComponent& plane_SM = coord.GetComponent<ModelOne::StaticMeshComponent>(plane);
		coord.GroupEntity(plane, "Static");
		ModelOne::Application::Get().GetMeshLibrary().Load(plane_SM.meshName, "assets/3dmodels/cube.obj");
		ModelOne::Application::Get().GetTextureLibrary().Load(plane_SM.textureName, "assets/textures/paving.png");
		//----------------------------------//





		/* CREATES THE CAMERA */
		ModelOne::Entity player = coord.CreateEntity();
		coord.AddComponent<ModelOne::CameraComponent>(player, 45.0f, 16.0f / 9.0f, 0.1f, 100.0f);
		coord.AddComponent<ModelOne::TransformComponent>(player);

		std::unordered_map<int, SandboxApp::input_func> keys_functions = {
			{MO_KEY_A, SandboxApp::MovePlayer}, {MO_KEY_D, SandboxApp::MovePlayer},
			{MO_KEY_W, SandboxApp::MovePlayer}, {MO_KEY_S, SandboxApp::MovePlayer},
			{MO_KEY_Q, SandboxApp::MovePlayer}, {MO_KEY_E, SandboxApp::MovePlayer},
			{MO_KEY_SPACE, SandboxApp::MovePlayer},
			{MO_MOUSE_KEY, SandboxApp::RotatePlayer}
		};
		coord.AddComponent<SandboxApp::InputComponent>(player, keys_functions);
		coord.AddComponent<SandboxApp::PlayerMovement>(player);

		coord.GroupEntity(player, "camera");
		coord.GroupEntity(player, "Player");

		ModelOne::TransformComponent& transformCam = coord.GetComponent<ModelOne::TransformComponent>(player);
		transformCam.location = glm::vec3(0.0f, 0.0f, -10.0f);
		transformCam.rotation = glm::vec3(0.0f, 0.0f, 0.0f);

	};


	virtual void OnImGuiRender() {
		
		std::set<ModelOne::Entity> entities = coord.GetEntitiesByGroup("Movable");

		ImGui::Begin("Entities manager");
		ImGui::SeparatorText("Controls");

		if (ImGui::Button("Add new entity")) {
			SpawnAxolotl(); coord.Update();
		}
		ImGui::SameLine();
		if (ImGui::Button("Remove entity")) {
			if (entities.size() != 0) {
				int random_entity = std::rand() % entities.size();

				int index = 0;
				for each (ModelOne::Entity entity in entities) {
					if (index == random_entity) {
						coord.KillEntity(entity);
					}
					index++;
				}
			}
		}
		ImGui::SameLine();
		ImGui::TextDisabled("(?)");
		if (ImGui::BeginItemTooltip()) {
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted("Destroy a random entity");
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}

		ImGui::SeparatorText("Entities list");
		static ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;

		if (ImGui::BeginTable("entities", 3, flags)) {
			ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableSetupColumn("Texture", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn("Position", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableHeadersRow();
			for each (ModelOne::Entity entity in entities) {
				ModelOne::StaticMeshComponent& mesh_comp = coord.GetComponent<ModelOne::StaticMeshComponent>(entity);
				ModelOne::TransformComponent& transform_comp = coord.GetComponent<ModelOne::TransformComponent>(entity);
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("%d", entity.GetId());
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%s", mesh_comp.textureName.c_str());
				ImGui::TableSetColumnIndex(2);
				ImGui::Text("X: %f, Y: %f, Z: %f", transform_comp.location.x, transform_comp.location.y, transform_comp.location.z);

			}
			ImGui::EndTable();
		}
		ImGui::End();
	}

private:
	void SpawnAxolotl() {
		int random_texture = std::rand() % 5;
		glm::vec3 random_location = { (float)(std::rand() % 22) - 10, (float)(std::rand() % 12) - 2, (float)(std::rand() % 12) - 2 };
		glm::vec3 random_rotation = { (float)(std::rand() % 180), (float)(std::rand() % 180), (float)(std::rand() % 180) };
		glm::vec3 random_scale = glm::vec3(0.25f);

		// Create axolotl
		ModelOne::Entity axolotl = coord.CreateEntity();
		coord.AddComponent<ModelOne::TransformComponent>(axolotl, random_location, random_rotation, random_scale);
		coord.AddComponent<ModelOne::StaticMeshComponent>(axolotl, "object", texture_names[random_texture], "axolotl_mesh");
		coord.AddComponent<ModelOne::PointMassComponent>(axolotl, glm::vec3(0.0f), glm::vec3(0.0f), 0.9f, ((float)1.0f) / 20.0f);
		coord.AddComponent<ModelOne::BoxCollisionComponent>(axolotl, glm::vec3(0.05f, 0.05f, 0.05f),
			[&](int a, int b) {
				ModelOne::AudioCommand::PlayAudio(ModelOne::Application::Get().GetAudioLibrary().Get("sfx"), false, 1.0f, 0.0f);
			});
		coord.GroupEntity(axolotl, "Movable");
	}


private:

	ModelOne::Coordinator& coord = ModelOne::Application::Get().GetCoordinator();
	std::vector<std::string> texture_names = { "axolotl_cyan", "axolotl_gold", "axolotl_blue", "axolotl_lucy", "axolotl_wild" };
};



class AudioLoaderLayer : public ModelOne::Layer {
public:
	AudioLoaderLayer() : Layer("AudioLayer") {
		ModelOne::Application::Get().GetAudioLibrary().Load("sfx", "assets/audios/boing.wav");
		ModelOne::Application::Get().GetAudioLibrary().Load("music", "assets/audios/music.wav");

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


