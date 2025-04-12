#pragma once
#include "Walnut/Layer.h"
#include "HeadlessConsole.h"

#include "Walnut/Networking/Server.h"
#include <map>
#include <mutex>
#include <glm/glm.hpp>

namespace Cubed {

	class ServerLayer : public Walnut::Layer
	{
	public:
		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(float ts) override;
		virtual void OnUIRender() override;
	private:
		void OnConsoleMessage(std::string_view message);

		void OnClientConnected(const Walnut::ClientInfo& client);
		void OnClientDisconnected(const Walnut::ClientInfo& client);
		void OnDataReceived(const Walnut::ClientInfo& client, const Walnut::Buffer& buffer);
	private:
		HeadlessConsole m_Console;
		Walnut::Server m_Server{ 8192 };

		struct PlayerData {
			glm::vec2 Position;
			glm::vec2 Velocity;
		};

		// process: we lock a data, then use a data, then unlock a data.
		std::mutex m_PlayerDataMutex; // mutex for player data
		std::map<uint32_t, PlayerData> m_PlayerData;
	};

}