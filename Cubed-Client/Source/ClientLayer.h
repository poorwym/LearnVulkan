#pragma once
#include "Walnut/Layer.h"

#include "Walnut/Networking/Client.h"

#include <glm/glm.hpp>
#include <mutex>

namespace Cubed {

	class ClientLayer : public Walnut::Layer
	{
	public:
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(float ts) override;
		virtual void OnRender() override;
		virtual void OnUIRender() override;
	private:
		void OnDataReceived(const Walnut::Buffer buffer);
	private:
		glm::vec2 m_PlayerPosition = {50, 50};
		glm::vec2 m_PlayerVelocity = { 0, 0 };
		uint32_t m_PlayerID = 0; // ID of the player

		std::string m_ServerAddress;

		Walnut::Client m_Client;

		struct PlayerData {
			glm::vec2 Position;
			glm::vec2 Velocity;
		};

		// process: we lock a data, then use a data, then unlock a data.
		std::mutex m_PlayerDataMutex; // mutex for player data
		std::map<uint32_t, PlayerData> m_PlayerData;
	};
}
