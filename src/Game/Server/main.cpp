#include "GameServer.hpp"

int main()
{
    GameServer gameServer;

    while (true) {
        gameServer.update();
    }
}
