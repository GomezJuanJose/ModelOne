#pragma once
#include <ModelOne/Core/KeyCodes.h>

enum class ModelOne::EventType;
class ModelOne::MouseMovedEvent;

namespace SandboxApp {
	struct PlayerMovement;
	

	void MovePlayer(PlayerMovement& player_mov, int keycode, ModelOne::Event* e) {

		if (e->GetEventType() == ModelOne::EventType::KeyPressed) {
			if (keycode == MO_KEY_A) {
				player_mov.left_current_speed = -1.0f * player_mov.speed;
			}
			if (keycode == MO_KEY_D) {
				player_mov.right_current_speed = 1.0f * player_mov.speed;
			}
			if (keycode == MO_KEY_W) {
				player_mov.forward_current_speed = -1.0f * player_mov.speed;
			}
			if (keycode == MO_KEY_S) {
				player_mov.backward_current_speed = 1.0f * player_mov.speed;
			}
			if (keycode == MO_KEY_Q) {
				player_mov.up_current_speed = 1.0f * player_mov.speed;
			}
			if (keycode == MO_KEY_E) {
				player_mov.down_current_speed = -1.0f * player_mov.speed;
			}
			if (keycode == MO_KEY_SPACE) {
				player_mov.enable_camera_movement = true;
			}
		}
		else if (e->GetEventType() == ModelOne::EventType::KeyReleased) {
			if (keycode == MO_KEY_A) {
				player_mov.left_current_speed = 0;
			}
			if (keycode == MO_KEY_D) {
				player_mov.right_current_speed = 0;
			}
			if (keycode == MO_KEY_W) {
				player_mov.forward_current_speed = 0;
			}
			if (keycode == MO_KEY_S) {
				player_mov.backward_current_speed = 0;
			}
			if (keycode == MO_KEY_Q) {
				player_mov.up_current_speed = 0;
			}
			if (keycode == MO_KEY_E) {
				player_mov.down_current_speed = 0;
			}
			if (keycode == MO_KEY_SPACE) {
				player_mov.enable_camera_movement = false;
			}
		}
	}

	void RotatePlayer(PlayerMovement& player_mov, int keycode, ModelOne::Event* e) {
		ModelOne::MouseMovedEvent* mouse = (ModelOne::MouseMovedEvent*)e;
		if (mouse->GetX() > player_mov.x_axis) {
			player_mov.x_axis_speed = 1.0f * player_mov.sensivity;
		}
		else if (mouse->GetX() < player_mov.x_axis) {
			player_mov.x_axis_speed = -1.0f * player_mov.sensivity;
		}
		player_mov.x_axis = mouse->GetX();
		

		if (mouse->GetY() > player_mov.y_axis) {
			player_mov.y_axis_speed = 1.0f * player_mov.sensivity;
		}
		else if (mouse->GetY() < player_mov.y_axis) {
			player_mov.y_axis_speed = -1.0f * player_mov.sensivity;
		}
		player_mov.y_axis = mouse->GetY();
	}
}