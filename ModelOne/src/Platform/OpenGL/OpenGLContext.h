#pragma once

#include "ModelOne/Renderer/RenderContext.h"

struct GLFWwindow;

namespace ModelOne {
	class OpenGLContext : public RenderContext {
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* m_WindowHandle;
	};
}
