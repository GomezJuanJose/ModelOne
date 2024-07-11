#pragma once
//This file is for be include in an application as #include <Taller.h> and make it easer to use
#include "tlpch.h" //TODO This is because std is not found in others .h files and gives error to compile. Find more info
#include "Taller/Core/Application.h"
#include "Taller/Core/Layer.h"
#include "Taller/Debug/Log.h"

#include "Taller/Core/Timestep.h"

#include "Taller/FunctionsLibraries/ComponentOperations.h"

#include "Taller/Core/Input.h"
#include "Taller/Core/KeyCodes.h"
#include "Taller/Core/MouseButtonCodes.h"

#include "Taller/ImGui/ImGuiLayer.h"

#include "Taller/FunctionsLibraries/FileOperations.h"

/*  --- RENDERER ---   */
#include "Taller/Renderer/RenderCommand.h"
#include "Taller/Renderer/Renderer.h"

#include "Taller/Renderer/Buffer.h"
#include "Taller/Renderer/FrameBuffer.h"
#include "Taller/Renderer/Shader.h"
#include "Taller/Renderer/VertexArray.h"

#include "Taller/Renderer/Mesh.h"
#include "Taller/Renderer/Texture.h"
/* ------------------- */