#include <Taller.h>


class ExampleLayer : public Taller::Layer {
public:
	ExampleLayer() : Layer("Example") {};

	void OnUpdate() override {
		TL_LOG_INFO(true, "Example layer update!");
	}

	void OnEvent(Taller::Event& e) override {
		TL_LOG_INFO(true, "Layer event : %s", e.ToString());
	}
};

class Sandbox : public Taller::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
	}
	~Sandbox() {}

private:

};

Taller::Application* Taller::CreateApplication() {
	return new Sandbox();
}