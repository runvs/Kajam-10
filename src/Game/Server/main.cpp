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

        std::cout << "received packet from " << sender.toString() << ":" << sender_port
                  << std::endl;

        try {
            Network::Packets::deserializeTestPacket(packet, id, string);
            auto packet2 = Network::Packets::serializeTestPacket(++id, "Hans Wurst");
            socket.send(packet2, sender, Network::NetworkProperties::port() - 1);

        } catch (std::exception const& e) {
            std::cout << e.what() << std::endl;
        }
    }
    return 0;
}
