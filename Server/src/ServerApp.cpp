#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "ServerLayer.h"

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
    ApplicationSpecification spec;
    spec.Name = "Block Hunter Server";

    Application* app = new Application(spec);

    app->PushLayer<BlockHunter::ServerLayer>();

    return app;
}
