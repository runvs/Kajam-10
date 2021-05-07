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
        sf::IpAddress sender;
        unsigned short sender_port;
        auto const result = socket.receive(packet, sender, sender_port);
        if (result != sf::Socket::Status::Done) {
            std::cout << "connection terminated\n";
            break;
        }

        // TODO move (de-)serialization into common
        // TODO reuse socket for sending data
        try {
            Network::Packets::receiveTestPacket(packet, id, string);
        } catch (std::exception const& e) {
            std::cout << e.what() << std::endl;
        }
    }
    return 0;
}
