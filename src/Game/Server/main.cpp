#include "../Common/Payloads.hpp"
#include "../Common/common.hpp"
#include "NetworkServer.hpp"
#include <SFML/Network.hpp>
#include <iostream>

int main()
{
    std::cout << "server started\n";

    NetworkServer server;
    char a;
    std::cin >> a;
    std::map<std::size_t, int> positions;
    positions[0] = 100;
    server.send(PayloadServer2Client { 0, positions });
    std::cin >> a;
    return 0;
}
