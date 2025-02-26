#pragma once
//This file is for be include in an application as #include <ModelOne.h> and make it easer to use
#include "mopch.h" //TODO This is because std is not found in others .h files and gives error to compile. Find more info
#include "ModelOne/Core/Application.h"
#include "ModelOne/Core/Layer.h"
#include "ModelOne/Debug/Log.h"

#include "ModelOne/Core/Timestep.h"

#include "ModelOne/FunctionsLibraries/ComponentOperations.h"

#include "ModelOne/Core/Input.h"
#include "ModelOne/Core/KeyCodes.h"
#include "ModelOne/Core/MouseButtonCodes.h"

#include "ModelOne/ImGui/ImGuiLayer.h"

#include "ModelOne/FunctionsLibraries/FileOperations.h"

#include "ModelOne/ECS/ECS.h"
#include "ModelOne/ECS/Components.h"

/*  --- ASSETS MANAGERS ---  */
#include "ModelOne/Renderer/MeshLibrary.h"
#include "ModelOne/Renderer/Texture2DLibrary.h"
#include "ModelOne/Renderer/ShaderLibrary.h"
#include "ModelOne/Audio/SoundLibrary.h"

/*  --- RENDERER ---  */
#include "ModelOne/Renderer/RenderCommand.h"
#include "ModelOne/Renderer/Renderer.h"

#include "ModelOne/Renderer/Buffer.h"
#include "ModelOne/Renderer/FrameBuffer.h"
#include "ModelOne/Renderer/Shader.h"
#include "ModelOne/Renderer/VertexArray.h"

#include "ModelOne/Renderer/MeshLibrary.h"
#include "ModelOne/Renderer/Texture.h"
/* ------------------- */

#include "ModelOne/Audio/Audio.h"