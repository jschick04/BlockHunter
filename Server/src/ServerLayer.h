#pragma once

#include <glm/glm.hpp>
#include "HeadlessConsole.h"
#include "Walnut/Layer.h"
#include "Walnut/Networking/Server.h"

namespace BlockHunter
{
    class ServerLayer final : public Walnut::Layer
    {
    public:
        void OnAttach() override;
        void OnDetach() override;

        void OnUpdate(float ts) override;
        void OnUIRender() override;

    private:
        void OnConsoleMessage(std::string_view message);

        void OnClientConnected(const Walnut::ClientInfo& clientInfo);
        void OnClientDisconnected(const Walnut::ClientInfo& clientInfo);
        void OnDataReceived(const Walnut::ClientInfo& clientInfo, const Walnut::Buffer buffer);

    private:
        HeadlessConsole m_console;
        Walnut::Server m_server { 8192 };

        struct PlayerData
        {
            glm::vec2 Position;
            glm::vec2 Velocity;
        };

        std::mutex m_playerDataLock;
        std::map<uint32_t, PlayerData> m_playerData;
    };
}
