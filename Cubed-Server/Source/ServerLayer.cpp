#include "ServerLayer.h"
#include "Walnut/Core/Log.h"
#include "Walnut/Serialization/BufferStream.h"
#include "ServerPacket.h"

namespace Cubed
{
	static Walnut::Buffer s_ScratchBuffer;

	void ServerLayer::OnConsoleMessage(std::string_view message)
	{
		if (message.starts_with('/')) {
			// do something
			std::cout << "Command: " << message << std::endl;
		}
	}


	void ServerLayer::OnAttach()
	{
		s_ScratchBuffer.Allocate(1024 * 1024); // 10MB

		m_Console.SetMessageSendCallback([this](std::string_view msg) {OnConsoleMessage(msg); });

		m_Server.SetClientConnectedCallback([this](const Walnut::ClientInfo client) { OnClientConnected(client);});
		m_Server.SetClientDisconnectedCallback([this](const Walnut::ClientInfo client) {OnClientDisconnected(client); });
		m_Server.SetDataReceivedCallback([this](const Walnut::ClientInfo& client, const Walnut::Buffer& buffer) { OnDataReceived(client, buffer); });

		m_Server.Start();
	}

	void ServerLayer::OnDetach()
	{
		m_Server.Stop();
	}

	void ServerLayer::OnUpdate(float ts)
	{
		// 使用 mutex lock 来防止同时分发与修改数据同一数据(OnDataReceived和OnUpdate是跑在不同thread上的)
		Walnut::BufferStreamWriter stream(s_ScratchBuffer);
		stream.WriteRaw(PacketType::ClientUpdate);
		m_PlayerDataMutex.lock();
		{
			stream.WriteMap(m_PlayerData);
		}
		m_PlayerDataMutex.unlock();

		m_Server.SendBufferToAllClients(stream.GetBuffer());

		using namespace std::chrono_literals;
		std::this_thread::sleep_for(5ms); // sleep for 5ms
	}

	void ServerLayer::OnUIRender()
	{

	}

	void ServerLayer::OnClientConnected(const Walnut::ClientInfo& clientInfo)
	{
		WL_INFO_TAG("Server", "Client connected: ID={0}", clientInfo.ID);

		Walnut::BufferStreamWriter stream(s_ScratchBuffer);


		stream.WriteRaw(PacketType::ClientConnect);
		stream.WriteRaw(clientInfo.ID);

		// packet type - connected
		// id 

		m_Server.SendBufferToClient(clientInfo.ID, stream.GetBuffer());
	}

	void ServerLayer::OnClientDisconnected(const Walnut::ClientInfo& clientInfo)
	{
		WL_INFO_TAG("Server", "Client disconnected: ID={0}", clientInfo.ID);
	}

	void ServerLayer::OnDataReceived(const Walnut::ClientInfo& clientInfo, const Walnut::Buffer& buffer)
	{
		WL_INFO_TAG("Server", "Data received from client: ID={0}", clientInfo.ID);
		Walnut::BufferStreamReader stream(buffer);
		PacketType packetType;
		stream.ReadRaw(packetType);
		switch (packetType)
		{
		case PacketType::ClientConnect:
		{
			break;
		}
		case PacketType::ClientUpdate:
		{
			// 使用 mutex lock 来防止多个玩家同时访问同一数据
			m_PlayerDataMutex.lock();
			{
				PlayerData& playerData = m_PlayerData[clientInfo.ID];
				stream.ReadRaw<glm::vec2>(playerData.Position);
				stream.ReadRaw<glm::vec2>(playerData.Velocity);
			}
			m_PlayerDataMutex.unlock();
			break;
		}
		default:
			break;
		}

	}
} 