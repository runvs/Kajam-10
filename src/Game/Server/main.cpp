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

int main()
{
    std::cout << "server started\n";

    NetworkServer server;

    PlayerMap playerStates;
    std::map<int, int> player_prediction_id;

    while (true) {
        auto now = std::chrono::steady_clock::now();
        float const elapsed = 0.04f;
        auto next = now + std::chrono::milliseconds { static_cast<long long>(elapsed * 1000) };

        auto playerIds = server.getAllPlayerIds();

        removeInactivePlayers(playerStates, playerIds);

        for (auto pid : playerIds) {
            if (playerStates.count(pid) == 0) {
                playerStates[pid].position.x() = 0;
                playerStates[pid].position.y() = 0;
            }
            auto dataForPlayer = server.getData(pid);

            for (auto& playerData : dataForPlayer) {
                updatePlayerState(playerStates[pid], playerData.dt, playerData.input);
                player_prediction_id[pid] = playerData.currentPredictionId;
            }
        }

        PayloadServer2Client payload { 0, playerStates };
        server.send(payload);

        std::this_thread::sleep_until(next);
    }
}
