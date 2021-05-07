#include "common.hpp"
#include <iostream>

namespace Network {
namespace Packets {
sf::Packet createTestPacket(std::size_t id, std::string message)
{
    sf::Packet packet;
    packet << id << message;

    return packet;
}

void receiveTestPacket(sf::Packet packet, std::size_t& id, std::string& message)
{
    if (packet >> id >> message) {
        std::cout << "received message:\n\t" << id << "\t" << message << std::endl;
    } else {
        throw std::exception { "deserialization failed" };
    }
}
} // namespace Packets
} // namespace Network