#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Core/Log.h"

class ExampleLayer : public Walnut::Layer
{
public:
	virtual void OnAttach() override {};
	virtual void OnUpdate(float ts) override {
		WL_INFO("Hello Cubed");
	}
private:
	bool m_AboutModalOpen = false;
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Cubed";
	Walnut::Application* app = new Walnut::Application(spec);
	std::shared_ptr<ExampleLayer> exampleLayer = std::make_shared<ExampleLayer>();
	app->PushLayer(exampleLayer);
	return app;
}