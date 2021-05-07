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
        std::cout << "thread started\n";
        sf::UdpSocket socket;

        socket.bind(Network::NetworkProperties::port() - 1);

        auto packet = Network::Packets::serializeTestPacket(1, "ABCD");
        if (socket.send(packet, sf::IpAddress("127.0.0.1"), Network::NetworkProperties::port())
            != sf::Socket::Done) {
            std::cout << "error sending data\n";
        }
        std::cout << "data send\n";

        sf::Packet packet2;
        sf::IpAddress sender;
        unsigned short sender_port;
        socket.receive(packet2, sender, sender_port);
        std::size_t id;
        std::string message;
        Network::Packets::deserializeTestPacket(packet2, id, message);
        std::cout << "received answer " << id << std::endl;
    });
    thread.join();
    game->startGame(std::make_shared<StateMenu>(), gameloop);

    std::cout << "thread ended\n";
    return 0;
}
