#include "Payloads.hpp"
#include <iostream>
#include <sstream>

sf::Packet& operator<<(sf::Packet& packet, Payload& payload)
{
    return packet << payload.payloadType << payload.message;
}

sf::Packet& operator>>(sf::Packet& packet, Payload& payload)
{
    return packet >> payload.payloadType >> payload.message;
}

std::size_t getPayloadType(PayloadWelcomeResponse /*unused*/) { return 1; }

Payload convert(PayloadWelcomeResponse const& payload)
{
    Payload payloadOut { getPayloadType(payload), std::to_string(payload.id) };
    return payloadOut;
}

void convert(Payload const& payloadIn, PayloadWelcomeResponse& payloadOut)
{
    if (payloadIn.payloadType == getPayloadType(payloadOut)) {
        std::stringstream sstream(payloadIn.message);
        sstream >> payloadOut.id;
    } else {
        std::cout << "cannot convert payload. Types do not match\n";
    }
}
