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