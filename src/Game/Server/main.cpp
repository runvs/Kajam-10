#include "../Common/Payloads.hpp"
#include "../Common/common.hpp"
#include "NetworkServer.hpp"
#include <SFML/Network.hpp>
#include <iostream>
#include <string>

int main()
{
    std::cout << "server started\n";

    NetworkServer server;
    char a;
    std::cin >> a;
    std::map<std::size_t, int> positions;
    positions[0] = 100;
    positions[1] = 220;
    positions[2] = 330;
    server.send(PayloadServer2Client { positions });
    std::cin >> a;
    return 0;
}
