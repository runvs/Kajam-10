#include "../Common/Payloads.hpp"
#include "../Common/common.hpp"
#include "NetworkServer.hpp"
#include <SFML/Network.hpp>
#include <iostream>

int main()
{
    std::cout << "server started\n";
    /*
    NetworkServer server;
    char a;
    std::cin >> a;
    std::map<std::size_t, int> positions;
    positions[0] = 100;
    server.send(PayloadServer2Client { 0, positions });
    std::cin >> a;
    */

    NetworkServer server;

    std::map<int, float> playerPositions;

    while (true) {
        auto now = std::chrono::system_clock::now();
        float const elapsed = 0.04f;
        auto next = now + std::chrono::milliseconds { static_cast<long long>(elapsed * 1000) };

        auto playerIds = server.getAllPlayerIds();

        for (auto pid : playerIds) {
            if (playerPositions.count(pid) == 0) {
                playerPositions[pid] = 0;
            }
            auto playerData = server.getData(pid);
            if (std::get<0>(playerData)) {
                auto playerInput = std::get<1>(playerData);
                if (playerInput.input.size() != 0) {
                    playerPositions[pid] += elapsed * 100;
                }
            }
        }
        PayloadServer2Client payload { 0, playerPositions };
        server.send(payload);

        std::this_thread::sleep_until(next);
    }
    return 0;
}
