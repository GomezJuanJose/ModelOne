#pragma once
#include <ModelOne/Core/KeyCodes.h>
#include <ModelOne/ECS/ECS.h>
#include <unordered_map>

namespace SandboxApp {
	
	ModelOne::Event;

	struct PlayerMovement {

		float left_current_speed;
		float right_current_speed;
		float forward_current_speed;
		float backward_current_speed;
		float up_current_speed;
		float down_current_speed;

		float speed;

		float x_axis;
		float x_axis_speed;
		float y_axis;
		float y_axis_speed;

		float sensivity;

		bool enable_camera_movement;

		bool is_valid;

		PlayerMovement(float speed = 10.0f, float sensivity = 100.0f) :
			speed(speed), sensivity(sensivity),
			left_current_speed(0.0f), right_current_speed(0.0f), 
			forward_current_speed(0.0f), backward_current_speed(0.0f), 
			up_current_speed(0.0f), down_current_speed(0.0f),
			x_axis(0.0f), y_axis(0.0f), x_axis_speed(0.0f), y_axis_speed(0.0f), enable_camera_movement(false), is_valid(true) {
		}
	};


	using input_func = void(*)(PlayerMovement& player_mov, int keycode, ModelOne::Event* e);
	struct InputComponent {

		std::unordered_map<int, input_func> keys_functions;

		bool is_valid;

		InputComponent(std::unordered_map<int, input_func> keys_functions = {}) : keys_functions(keys_functions), is_valid(true) {
		}
	};
}