#include "NetworkServer.hpp"
#include "Payloads.hpp"
#include "PlayerState.hpp"
#include "SystemHelper.hpp"
#include "common.hpp"
#include <SFML/Network.hpp>
#include <iostream>

void removeInactivePlayers(PlayerMap& playerStates, std::vector<int> playerIds)
{
    if (playerIds.size() != playerStates.size()) {
        jt::SystemHelper::erase_if(playerStates, [&playerIds](auto const& kvp) {
            return std::find(playerIds.begin(), playerIds.end(), kvp.first) == playerIds.end();
        });
    }
}

void createNewPlayerIfNotKnownToServer(
    PlayerMap playerStates, std::vector<int>::value_type currentPlayerId)
{
    if (playerStates.count(currentPlayerId) == 0) {
        playerStates[currentPlayerId].position.x() = 0;
        playerStates[currentPlayerId].position.y() = 0;
    }
}

int main()
{
    NetworkServer server;

    PlayerMap playerStates;
    std::map<int, std::size_t> playerPredictionId;

    std::cout << "server started\n";
    while (true) {
        auto now = std::chrono::steady_clock::now();

        auto next = now
            + std::chrono::milliseconds { static_cast<long long>(
                Network::NetworkProperties::serverTickTime() * 1000) };

        auto allPlayerIds = server.getAllPlayerIds();

        removeInactivePlayers(playerStates, allPlayerIds);

        std::vector<int> playersToDisconnect {};
        for (auto currentPlayerId : allPlayerIds) {

            createNewPlayerIfNotKnownToServer(playerStates, currentPlayerId);

            auto dataForPlayer = server.getData(currentPlayerId);
            // TODO Sort dataForPlayer by message id
            // TODO remove duplicated messages by message id
            for (auto& payload : dataForPlayer) {
                if (payload.disconnect == true) {
                    playersToDisconnect.push_back(currentPlayerId);
                    break;
                }
                updatePlayerState(playerStates[currentPlayerId], payload.dt, payload.input);
                playerPredictionId[currentPlayerId] = payload.currentPredictionId;
                // std::cout << payload.currentPredictionId << std::endl;
            }
        }

        for (auto playerToDisconnectId : playersToDisconnect) {
            server.closeConnectionTo(playerToDisconnectId);
        }

        for (auto& kvp : playerStates) {
            PayloadServer2Client payload { kvp.first, playerStates, playerPredictionId[kvp.first] };
            server.sendToClient(kvp.first, payload);
        }

        std::this_thread::sleep_until(next);
    }
}
