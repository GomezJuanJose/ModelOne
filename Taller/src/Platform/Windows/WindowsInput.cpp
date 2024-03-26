#include "tlpch.h"
#include "WindowsInput.h"

#include "Taller/Application.h"
#include <GLFW/glfw3.h>

namespace Taller {
	Input* Input::s_Instance = new WindowsInput();

	bool WindowsInput::IsKeyPressedImplementation(int keycode) {
		Window& window = Application::Get().GetWindow();
		GLFWwindow* nativeWindow = static_cast<GLFWwindow*>(window.GetNativeWindow());

		auto state = glfwGetKey(nativeWindow, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool WindowsInput::IsMouseButtonImplementation(int button) {
		Window& window = Application::Get().GetWindow();
		GLFWwindow* nativeWindow = static_cast<GLFWwindow*>(window.GetNativeWindow());

		auto state = glfwGetKey(nativeWindow, button);
		return state == GLFW_PRESS;
	}

	std::pair<float, float> WindowsInput::GetMousePositionImplementation() {
		Window& window = Application::Get().GetWindow();
		GLFWwindow* nativeWindow = static_cast<GLFWwindow*>(window.GetNativeWindow());

		double xpos, ypos;
		glfwGetCursorPos(nativeWindow, &xpos, &ypos);

		return { (float)xpos, (float)ypos };
	}

	float WindowsInput::GetMouseXImplementation() {
		auto [x, y] = GetMousePositionImplementation();
		return x;
	}

	float WindowsInput::GetMouseYImplementation() {
		auto [x, y] = GetMousePositionImplementation();
		return y;
	}
}