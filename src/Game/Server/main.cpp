﻿#include "../Common/Payloads.hpp"
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
    server.send(convert(PayloadWelcomeResponse { 15 }));
    std::cin >> a;
    return 0;
}