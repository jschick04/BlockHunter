#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"
#include "Walnut/Core/Log.h"

class ExampleLayer final : public Walnut::Layer
{
public:
    void OnAttach() override { }

    void OnUpdate(float ts) override
    {
        WL_INFO("Server Timestep: {}", ts);

        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
    Walnut::ApplicationSpecification spec;
    spec.Name = "Block Hunter Server";

    Walnut::Application* app = new Walnut::Application(spec);

    std::shared_ptr<ExampleLayer> layer = std::make_shared<ExampleLayer>();
    app->PushLayer(layer);

    return app;
}
