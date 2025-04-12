#include "ClientLayer.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "misc/cpp/imgui_stdlib.h"

#include "Walnut/Input/Input.h"
#include "Walnut/ImGui/ImGuiTheme.h"
#include "Walnut/Input/KeyCodes.h"

#include "Walnut/Serialization/BufferStream.h"
#include "ServerPacket.h"

#include "Walnut/Core/Log.h"

namespace Cubed {

	static Walnut::Buffer s_ScratchBuffer;

	static void DrawRect(glm::vec2 position, glm::vec2 size, uint32_t color)
	{
		ImDrawList* drawList = ImGui::GetBackgroundDrawList();
		ImVec2 min = ImGui::GetWindowPos() + ImVec2(position.x, position.y);
		ImVec2 max = min + ImVec2(size.x, size.y);

		drawList->AddRectFilled(min, max, color);
	}

	void ClientLayer::OnAttach()
	{
		s_ScratchBuffer.Allocate(10 * 1024 * 1024);

		m_Client.SetDataReceivedCallback([this](const Walnut::Buffer buffer) { OnDataReceived(buffer);});

		
	}

	void ClientLayer::OnDetach()
	{

	}

	void ClientLayer::OnUpdate(float ts)
	{
		if (m_Client.GetConnectionStatus() != Walnut::Client::ConnectionStatus::Connected)
			return;

		glm::vec2 dir = { 0.0f, 0.0f };
		if (Walnut::Input::IsKeyDown(Walnut::KeyCode::W)) {
			dir.y = -1.0f;
		}
		else if (Walnut::Input::IsKeyDown(Walnut::KeyCode::S)){
			dir.y = 1.0f;
		}
		if (Walnut::Input::IsKeyDown(Walnut::KeyCode::A)) {
			dir.x = -1.0f;
		}
		else if (Walnut::Input::IsKeyDown(Walnut::KeyCode::D)) {
			dir.x = 1.0f;
		}

		if (dir != glm::vec2(0.0f))
		{
			const float speed = 5.0f;
			m_PlayerVelocity = dir * speed;
		}

		m_PlayerVelocity = glm::mix(m_PlayerVelocity, glm::vec2(0.0f, 0.0f), 0.3f);

		m_PlayerPosition += m_PlayerVelocity * (ts * 1000);

		Walnut::BufferStreamWriter stream(s_ScratchBuffer);

		// Write the packet type
		stream.WriteRaw(PacketType::ClientUpdate);
		stream.WriteRaw<glm::vec2>(m_PlayerPosition);
		stream.WriteRaw<glm::vec2>(m_PlayerVelocity);
		m_Client.SendBuffer(stream.GetBuffer());
	}

	void ClientLayer::OnRender()
	{
		 
	}

	void ClientLayer::OnUIRender()
	{
		Walnut::Client::ConnectionStatus connectionStatus = m_Client.GetConnectionStatus();
		if (connectionStatus == Walnut::Client::ConnectionStatus::Connected)
		{	
			DrawRect(m_PlayerPosition, { 50, 50 }, IM_COL32(255, 0, 0, 255));

			m_PlayerDataMutex.lock();
			std::map<uint32_t, PlayerData> playerData = m_PlayerData; // copy the data
			m_PlayerDataMutex.unlock();

			for (const auto& [id, data] : playerData)
			{
				if (id == m_PlayerID)
					continue;
				DrawRect(data.Position, { 50.0f, 50.0f }, IM_COL32(0, 255, 0, 255));
				std::cout << "There is another player at " << data.Position.x << ", " << data.Position.y << std::endl;
			}
		}
		else
		{
			
			ImGui::Begin("Connect to server");

			
			ImGui::InputText("Server Address", &m_ServerAddress); 
			if(connectionStatus == Walnut::Client::ConnectionStatus::FailedToConnect)
			{
				ImGui::TextColored(ImColor(Walnut::UI::Colors::Theme::error), "Failed to connected to the server");
			}
			else if ( connectionStatus == Walnut::Client::ConnectionStatus::Connecting)
			{
				ImGui::TextColored(ImColor(Walnut::UI::Colors::Theme::textDarker), "Connecting...");
			}
			
				
			if (ImGui::Button("Connect"))
			{
				m_Client.ConnectToServer(m_ServerAddress);
			}

			ImGui::End();
		}
	}

	void ClientLayer::OnDataReceived(const Walnut::Buffer buffer)
	{
		Walnut::BufferStreamReader stream(buffer);
		PacketType packetType;
		stream.ReadRaw(packetType);

		switch (packetType)
		{
		case PacketType::ClientConnect:
		{
			uint32_t idFromServer;
			stream.ReadRaw<uint32_t>(idFromServer);
			// WL_INFO("We have connected! Server says our Id is {}", idFromServer);
			// WL_INFO("We say our Id is {}", m_Client.GetID());
			m_PlayerID = idFromServer;
			break;
		} 
		case PacketType::ClientUpdate:
		{
			// list all other
			m_PlayerDataMutex.lock();
			{
				stream.ReadMap(m_PlayerData);
			}
			m_PlayerDataMutex.unlock();
			break;
		}
		default:
			break;
		}
	}

}