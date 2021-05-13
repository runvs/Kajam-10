﻿#include "NetworkServer.hpp"
#include "Payloads.hpp"
#include "PlayerState.hpp"
#include "SystemHelper.hpp"
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
    std::cout << "starting server...\n";

    NetworkServer server;

    PlayerMap playerStates;
    std::map<int, std::size_t> player_prediction_id;

    while (true) {
        auto now = std::chrono::steady_clock::now();
        float const elapsed = 0.01f;
        auto next = now + std::chrono::milliseconds { static_cast<long long>(elapsed * 1000) };

        auto allPlayerIds = server.getAllPlayerIds();

        removeInactivePlayers(playerStates, allPlayerIds);

        std::vector<int> playersToDisconnect {};
        for (auto currentPlayerId : allPlayerIds) {

            createNewPlayerIfNotKnownToServer(playerStates, currentPlayerId);

            auto dataForPlayer = server.getData(currentPlayerId);
            // TODO Sort dataForPlayer by message id
            for (auto& payload : dataForPlayer) {
                if (payload.disconnect == true) {
                    playersToDisconnect.push_back(currentPlayerId);
                    break;
                }
                updatePlayerState(playerStates[currentPlayerId], payload.dt, payload.input);
                player_prediction_id[currentPlayerId] = payload.currentPredictionId;
                // std::cout << payload.currentPredictionId << std::endl;
            }
        }
        for (auto playerToDisconnectId : playersToDisconnect) {
            server.closeConnectionTo(playerToDisconnectId);
        }

        for (auto& kvp : playerStates) {
            PayloadServer2Client payload { kvp.first, playerStates,
                player_prediction_id[kvp.first] };
            server.sendToClient(kvp.first, payload);
        }

        std::this_thread::sleep_until(next);
    }
}
