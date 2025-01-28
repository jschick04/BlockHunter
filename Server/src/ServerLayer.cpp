#include "ServerLayer.h"

#include "ServerPacket.h"
#include "Walnut/Core/Log.h"
#include "Walnut/Serialization/BufferStream.h"

namespace BlockHunter
{
    static Walnut::Buffer s_scratchBuffer;

    void ServerLayer::OnAttach()
    {
        s_scratchBuffer.Allocate(10 * 1024 * 1024);

        m_console.SetMessageSendCallback([this](const std::string_view message) { OnConsoleMessage(message); });

        m_server.SetClientConnectedCallback(
            [this](const Walnut::ClientInfo& clientInfo)
            {
                OnClientConnected(clientInfo);
            });

        m_server.SetClientDisconnectedCallback(
            [this](const Walnut::ClientInfo& clientInfo)
            {
                OnClientDisconnected(clientInfo);
            });

        m_server.SetDataReceivedCallback(
            [this](const Walnut::ClientInfo& clientInfo, const Walnut::Buffer buffer)
            {
                OnDataReceived(clientInfo, buffer);
            });

        m_server.Start();
    }

    void ServerLayer::OnDetach()
    {
        m_server.Stop();
    }

    void ServerLayer::OnUpdate(float ts)
    {
        Walnut::BufferStreamWriter stream(s_scratchBuffer);
        stream.WriteRaw(PacketType::ClientUpdate);

        m_playerDataLock.lock();

        stream.WriteMap(m_playerData);

        m_playerDataLock.unlock();

        m_server.SendBufferToAllClients(stream.GetBuffer());

        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    void ServerLayer::OnUIRender() { }

    void ServerLayer::OnConsoleMessage(const std::string_view message)
    {
        if (message.starts_with('/'))
        {
            std::cout << "You called the " << message << " command!\n";
        }
    }

    void ServerLayer::OnClientConnected(const Walnut::ClientInfo& clientInfo)
    {
        WL_INFO_TAG("Server", "Client connected! ID={}", clientInfo.ID);

        Walnut::BufferStreamWriter stream(s_scratchBuffer);
        stream.WriteRaw(PacketType::ClientConnect);
        stream.WriteRaw(clientInfo.ID);

        m_server.SendBufferToClient(clientInfo.ID, stream.GetBuffer());
    }

    void ServerLayer::OnClientDisconnected(const Walnut::ClientInfo& clientInfo)
    {
        WL_INFO_TAG("Server", "Client disconnected! ID={}", clientInfo.ID);
    }

    void ServerLayer::OnDataReceived(const Walnut::ClientInfo& clientInfo, const Walnut::Buffer buffer)
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
                {
                    PlayerData& playerData = m_playerData[clientInfo.ID];
                    stream.ReadRaw(playerData.Position);
                    stream.ReadRaw(playerData.Velocity);
                }
                m_playerDataLock.unlock();

                break;
            case PacketType::ConnectionStatus:
            case PacketType::ClientList:
            case PacketType::ClientConnect:
            case PacketType::ClientDisconnect:
            case PacketType::ClientUpdateResponse:
            case PacketType::MessageHistory:
            case PacketType::ServerShutdown:
            case PacketType::ClientKick:
                break;
        }
    }
}
