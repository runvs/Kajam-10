#ifndef PAYLOADS_HPP_GUARD
#define PAYLOADS_HPP_GUARD

#include <SFML/Network.hpp>
#include <cstddef>
#include <string>

struct Payload {
    std::size_t payloadType;

    std::string message;
};

struct PayloadWelcomeRequest {
};

struct PayloadWelcomeResponse {
    size_t id;
};

sf::Packet& operator<<(sf::Packet&, Payload);
sf::Packet& operator>>(sf::Packet&, Payload);

#endif // PAYLOADS_HPP_GUARD
