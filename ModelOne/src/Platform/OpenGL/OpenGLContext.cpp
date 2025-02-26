#include "mopch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace ModelOne {
	
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle) {
		MO_ASSERT(m_WindowHandle, "OpenGLContext.cpp Windows handle NULL")
	}

	void OpenGLContext::Init() {

		MO_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		MO_ASSERT(status, "Failed to initialize Glad!");
	}
	void OpenGLContext::SwapBuffers() {

		MO_PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);
	}
}