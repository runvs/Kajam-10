#include "GameServer.hpp"
#include "common.hpp"
#include <iostream>

int main()
{
    GameServer gameServer;

    std::cout << "starting server...\n";

    while (true) {
        gameServer.update();
    }
}
