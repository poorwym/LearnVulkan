#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/UI/UI.h"
#include "ClientLayer.h"
#include <iostream>

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	std::cout << "Creating application..." << std::endl;
	Walnut::ApplicationSpecification spec;
	spec.Name = "Walnut Example";
	spec.CustomTitlebar = false;
	spec.UseDockspace = false;

	Walnut::Application* app = new Walnut::Application(spec);
	std::cout << "Application created" << std::endl;
	app->PushLayer<Cubed::ClientLayer>();
	std::cout << "Layer pushed to application" << std::endl;
	return app;
}