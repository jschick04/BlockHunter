#include "ClientLayer.h"

#include "ServerPacket.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "Walnut/Input/Input.h"
#include "Walnut/Serialization/BufferStream.h"
#include "misc/cpp/imgui_stdlib.h"

namespace BlockHunter
{
    static Walnut::Buffer s_scratchBuffer;

    static void DrawRect(const glm::vec2 position, const glm::vec2 size, const uint32_t color)
    {
        ImDrawList* drawList = ImGui::GetBackgroundDrawList();

        const ImVec2 min = ImGui::GetWindowPos() + ImVec2(position.x, position.y);
        const ImVec2 max = min + ImVec2(size.x, size.y);

        drawList->AddRectFilled(min, max, color);
    }

    void ClientLayer::OnAttach()
    {
        s_scratchBuffer.Allocate(10 * 1024 * 1024);

        m_client.SetDataReceivedCallback([this](const Walnut::Buffer buffer) { OnDataReceived(buffer); });
    }

    void ClientLayer::OnDetach() { }

    void ClientLayer::OnUpdate(const float ts)
    {
        if (m_client.GetConnectionStatus() != Walnut::Client::ConnectionStatus::Connected)
        {
            return;
        }

        glm::vec2 dir { 0.0f, 0.0f };

        if (Walnut::Input::IsKeyDown(Walnut::KeyCode::W))
        {
            dir.y = -1;
        }
        else if (Walnut::Input::IsKeyDown(Walnut::KeyCode::S))
        {
            dir.y = 1;
        }

        if (Walnut::Input::IsKeyDown(Walnut::KeyCode::A))
        {
            dir.x = -1;
        }
        else if (Walnut::Input::IsKeyDown(Walnut::KeyCode::D))
        {
            dir.x = 1;
        }

        if (glm::length(dir) > 0.0f)
        {
            constexpr float speed = 150.0f;
            dir = glm::normalize(dir);
            m_playerVelocity = dir * speed;
        }

        m_playerPosition += m_playerVelocity * ts;

        m_playerVelocity = glm::mix(m_playerVelocity, glm::vec2(0.0f), 10.0f * ts);

        Walnut::BufferStreamWriter stream(s_scratchBuffer);
        stream.WriteRaw(PacketType::ClientUpdate);
        stream.WriteRaw(m_playerPosition);
        stream.WriteRaw(m_playerVelocity);

        m_client.SendBuffer(stream.GetBuffer());
    }

    void ClientLayer::OnUIRender()
    {
        if (m_client.GetConnectionStatus() == Walnut::Client::ConnectionStatus::Connected)
        {
            DrawRect(m_playerPosition, { 50.0f, 50.0f }, 0xffff00ff);

            m_playerDataLock.lock();
            std::map<uint32_t, PlayerData> playerData = m_playerData;
            m_playerDataLock.unlock();

            for (const auto& [id, data] : playerData)
            {
                if (id == m_playerID) { continue; }

                DrawRect(data.Position, { 50.0f, 50.0f }, 0xff00ff00);
            }
        }
        else
        {
            ImGui::Begin("Connect to Server");

            ImGui::InputText("Server Address", &m_serverAddress);

            if (m_client.GetConnectionStatus() == Walnut::Client::ConnectionStatus::FailedToConnect)
            {
                ImGui::TextColored({ 255, 0, 0, 255 }, "Failed to connect");
            }
            else if (m_client.GetConnectionStatus() == Walnut::Client::ConnectionStatus::Connecting)
            {
                ImGui::TextColored({0, 255, 0, 255}, "Connecting...");
            }

            if (ImGui::Button("Connect"))
            {
                m_client.ConnectToServer(m_serverAddress);
            }

            ImGui::End();
        }
    }

    void ClientLayer::OnDataReceived(const Walnut::Buffer buffer)
    {
        PacketType type;

        Walnut::BufferStreamReader stream(buffer);
        stream.ReadRaw(type);

        switch (type)
        {
            case PacketType::None:
            case PacketType::Message:
            case PacketType::ClientConnectionRequest:
                break;
            case PacketType::ClientUpdate:
                m_playerDataLock.lock();

                stream.ReadMap(m_playerData);

                m_playerDataLock.unlock();

                break;
            case PacketType::ConnectionStatus:
            case PacketType::ClientList:
                break;
            case PacketType::ClientConnect:
                stream.ReadRaw(m_playerID);

                WL_INFO("We have connected!");

                break;
            case PacketType::ClientDisconnect:
            case PacketType::ClientUpdateResponse:
            case PacketType::MessageHistory:
            case PacketType::ServerShutdown:
            case PacketType::ClientKick:
                break;
        }
    }
}
