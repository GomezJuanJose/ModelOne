# Model One

[![MIT License](https://img.shields.io/badge/License-MIT-green.svg)](https://choosealicense.com/licenses/mit/)

## Description
ModelOne is primarily an early-stage game engine for Windows written in C++. Available features are::
+ Layered architecture
+ ECS
+ Renderer with OpenGL
+ GLSL support
+ OBJ loader
+ Basic physics system
+ Event system
+ Logger and assertions
+ Dear ImGui support
+ Audio
+ Profiler


## Installation
Visual Studio 2019 or later is recommended, ModelOne is untested in other development environments and focuses on Windows builds.

**1. Clone the repository**
```bash
  git clone --recursive https://github.com/GomezJuanJose/ModelOne.git
```

**2. Generate files**

Execute the script `Windows-ProjectGeneration.bat` located in the folder `/Scripts`. After the execution, open `ModelOne.sln` and press F5.


## Getting started
Open the `EmptyApp` proyect from the solution and mark it as the started proyect. It contains the basics to create a window with all the systems initialized:
```c++
#include <ModelOne.h>
#include <ModelOne/Core/EntryPoint.h>

class ExampleLayer : public ModelOne::Layer {
public:
	ExampleLayer() : Layer("ExampleLayer") {
	}

	virtual void OnImGuiRender(){
	}
};

class Sandbox : public ModelOne::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
	}
	~Sandbox() {}

};

ModelOne::Application* ModelOne::CreateApplication() {
	return new Sandbox();
}
```
The `Sandbox` example shows how to use the systems available.

## Create a new proyect
1. Copy the `EmptyApp`, paste it in `/SandboxProjects` and rename it
2. Open `ProjectDefinition.lua`, copy a existing project and paste it at the bottom of the file
3. Change this two attributes by the name of the project
```lua
project "<Name of the project>"
    location "SandboxProjects/<Name of the project>"
```
4. Execute the `Windows-ProjectGeneration.bat` located in the folder `/Scripts`

## Documentation

[https://github.com/GomezJuanJose/ModelOne/wiki](https://github.com/GomezJuanJose/ModelOne/wiki)

