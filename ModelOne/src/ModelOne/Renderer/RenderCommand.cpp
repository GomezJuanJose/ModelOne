#include "mopch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace ModelOne {
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}