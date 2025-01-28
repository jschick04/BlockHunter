#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "ClientLayer.h"

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	ApplicationSpecification spec;
	spec.Name = "Block Hunter";
	spec.CustomTitlebar = false;
	spec.UseDockspace = false;

	Application* app = new Application(spec);

	app->PushLayer<BlockHunter::ClientLayer>();
	
	return app;
}