#include "tlpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Taller {
	
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle) {
		TL_ASSERT(m_WindowHandle, "OpenGLContext.cpp Windows handle NULL")
	}

	void OpenGLContext::Init() {

		TL_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		TL_ASSERT(status, "Failed to initialize Glad!");
	}
	void OpenGLContext::SwapBuffers() {

		TL_PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);
	}
}