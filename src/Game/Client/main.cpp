#include "main.hpp"
#include "../common/common.hpp"
#include "Game.hpp"
#include "GameProperties.hpp"
#include "InputManager.hpp"
#include "KeyboardInput.hpp"
#include "MouseInput.hpp"
#include "MusicPlayer.hpp"
#include "Random.hpp"
#include "RenderWindow.hpp"
#include "StateMenu.hpp"

#include <SFML/Network.hpp>
#include <iostream>
#include <thread>

std::shared_ptr<jt::GameInterface> game;

void gameloop()
{
    if (game) {
        game->run();
    }
}

int main()
{
    hideConsoleInRelease();

    jt::Random::useTimeAsRandomSeed();
    auto const mouse = std::make_shared<jt::MouseInput>();
    auto const keyboard = std::make_shared<jt::KeyboardInput>();
    auto input = std::make_shared<jt::InputManager>(mouse, keyboard);

    game = std::make_shared<jt::Game>(std::make_shared<jt::RenderWindow>(GP::GetWindowSize().x(),
                                          GP::GetWindowSize().y(), GP::GameName()),
        GP::GetZoom(), input, std::make_shared<jt::MusicPlayer>());
    game->setupRenderTarget();

    // TODO refactor threading
    std::thread thread([]() {
        // TODO reuse socket for receiving data
        std::cout << "thread started\n";
        sf::UdpSocket socket;
        sf::Packet packet;
        packet << std::size_t { 1 } << std::string { "ABCD" };
        if (socket.send(packet, sf::IpAddress("127.0.0.1"), Network::NetworkProperties::port())
            != sf::Socket::Done) {
            std::cout << "error sending data\n";
        }
        std::cout << "data send\n";
    });
    thread.join();
    game->startGame(std::make_shared<StateMenu>(), gameloop);

    std::cout << "thread ended\n";
    return 0;
}
