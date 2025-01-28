#pragma once

#include <mutex>
#include <glm/glm.hpp>
#include "Walnut/Layer.h"
#include "Walnut/Networking/Client.h"

namespace BlockHunter
{
    class ClientLayer final : public Walnut::Layer
    {
    public:
        void OnAttach() override;
        void OnDetach() override;

        void OnUpdate(float ts) override;
        void OnUIRender() override;

    private:
        void OnDataReceived(const Walnut::Buffer buffer);

    private:
        glm::vec2 m_playerPosition { 50, 50 };
        glm::vec2 m_playerVelocity { 0, 0 };

        Walnut::Client m_client;
        uint32_t m_playerID = 0;

        std::string m_serverAddress;

        struct PlayerData
        {
            glm::vec2 Position;
            glm::vec2 Velocity;
        };

        std::mutex m_playerDataLock;
        std::map<uint32_t, PlayerData> m_playerData;
    };
}
