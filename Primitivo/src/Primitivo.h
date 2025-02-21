#pragma once
//This file is for be include in an application as #include <Primitivo.h> and make it easer to use
#include "plpch.h" //TODO This is because std is not found in others .h files and gives error to compile. Find more info
#include "Primitivo/Core/Application.h"
#include "Primitivo/Core/Layer.h"
#include "Primitivo/Debug/Log.h"

#include "Primitivo/Core/Timestep.h"

#include "Primitivo/FunctionsLibraries/ComponentOperations.h"

#include "Primitivo/Core/Input.h"
#include "Primitivo/Core/KeyCodes.h"
#include "Primitivo/Core/MouseButtonCodes.h"

#include "Primitivo/ImGui/ImGuiLayer.h"

#include "Primitivo/FunctionsLibraries/FileOperations.h"

#include "Primitivo/ECS/ECS.h"
#include "Primitivo/ECS/Components.h"

/*  --- ASSETS MANAGERS ---  */
#include "Primitivo/Renderer/MeshLibrary.h"
#include "Primitivo/Renderer/Texture2DLibrary.h"
#include "Primitivo/Renderer/ShaderLibrary.h"
#include "Primitivo/Audio/SoundLibrary.h"

/*  --- RENDERER ---  */
#include "Primitivo/Renderer/RenderCommand.h"
#include "Primitivo/Renderer/Renderer.h"

#include "Primitivo/Renderer/Buffer.h"
#include "Primitivo/Renderer/FrameBuffer.h"
#include "Primitivo/Renderer/Shader.h"
#include "Primitivo/Renderer/VertexArray.h"

#include "Primitivo/Renderer/MeshLibrary.h"
#include "Primitivo/Renderer/Texture.h"
/* ------------------- */

#include "Primitivo/Audio/Audio.h"