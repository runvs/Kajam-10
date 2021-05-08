#include "Payloads.hpp"

sf::Packet& operator<<(sf::Packet& packet, Payload payload)
{
    return packet << payload.payloadType << payload.message;
}

sf::Packet& operator>>(sf::Packet& packet, Payload payload)
{
    return packet >> payload.payloadType >> payload.message;
}