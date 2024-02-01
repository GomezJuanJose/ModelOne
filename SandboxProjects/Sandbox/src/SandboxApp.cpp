#include <Taller.h>

class Sandbox : public Taller::Application {
public:
	Sandbox() {}
	~Sandbox() {}

private:

};

Taller::Application* Taller::CreateApplication() {
	return new Sandbox();
}