#include "../Common/common.hpp"
#include <SFML/Network.hpp>
#include <iostream>
#include <string>

int main()
{
    std::cout << "server started\n";

    // accept a new connection
    sf::UdpSocket socket;
    auto status = socket.bind(Network::NetworkProperties::port());

    while (true) {
        std::size_t id = 0;
        std::string string;

        sf::Packet packet;
        sf::SocketSelector selector();
        sf::IpAddress sender;
        unsigned short sender_port;
        auto const result = socket.receive(packet, sender, sender_port);
        if (result != sf::Socket::Status::Done) {
            std::cout << "connection terminated\n";
            break;
        }

        packet >> id >> string;

        std::cout << "received message:\n\t" << id << "\t" << string << std::endl;
    }
    return 0;
}
