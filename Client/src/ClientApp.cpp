#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/UI/UI.h"

class ExampleLayer final : public Walnut::Layer
{
public:
	void OnUIRender() override { }
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	ApplicationSpecification spec;
	spec.Name = "Block Hunter";
	spec.CustomTitlebar = false;
	spec.UseDockspace = false;

	Application* app = new Application(spec);

	const std::shared_ptr<ExampleLayer> exampleLayer = std::make_shared<ExampleLayer>();
	app->PushLayer(exampleLayer);
	
	return app;
}