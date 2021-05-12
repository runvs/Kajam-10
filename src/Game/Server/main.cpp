#include "../Common/Payloads.hpp"
#include "../Common/PlayerState.hpp"
#include "NetworkServer.hpp"
#include "SystemHelper.hpp"
#include <SFML/Network.hpp>
#include <iostream>

void removeInactivePlayers(PlayerMap& playerStates, std::vector<int> playerIds)
{
    jt::SystemHelper::erase_if(playerStates, [&playerIds](auto const& kvp) {
        return std::find(playerIds.begin(), playerIds.end(), kvp.first) == playerIds.end();
    });
}

int main()
{
    std::cout << "server started\n";

    NetworkServer server;

    PlayerMap playerStates;

    while (true) {
        auto now = std::chrono::steady_clock::now();
        float const elapsed = 0.04f;
        auto next = now + std::chrono::milliseconds { static_cast<long long>(elapsed * 1000) };

        auto playerIds = server.getAllPlayerIds();
        if (playerIds.size() != playerStates.size()) {
            removeInactivePlayers(playerStates, playerIds);
        }
        for (auto pid : playerIds) {
            if (playerStates.count(pid) == 0) {
                playerStates[pid].position.x() = 0;
                playerStates[pid].position.y() = 0;
            }
            auto playerData = server.getData(pid);
            if (std::get<0>(playerData)) {
                auto playerInput = std::get<1>(playerData);
                if (playerInput.input[jt::KeyCode::D])
                    playerStates[pid].position.x() += elapsed * 100;
                else if (playerInput.input[jt::KeyCode::A])
                    playerStates[pid].position.x() -= elapsed * 100;

                if (playerInput.input[jt::KeyCode::W])
                    playerStates[pid].position.y() -= elapsed * 100;
                else if (playerInput.input[jt::KeyCode::S])
                    playerStates[pid].position.y() += elapsed * 100;
            }
        }

        PayloadServer2Client payload { 0, playerStates };
        server.send(payload);

        std::this_thread::sleep_until(next);
    }
}
