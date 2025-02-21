#include "plpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Primitivo {
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}