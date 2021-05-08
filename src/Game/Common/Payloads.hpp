#ifndef PAYLOADS_HPP_GUARD
#define PAYLOADS_HPP_GUARD

#include <SFML/Network.hpp>
#include <cstddef>
#include <string>

struct Payload {
    std::size_t payloadType;

    std::string message;
};

sf::Packet& operator<<(sf::Packet&, Payload&);
sf::Packet& operator>>(sf::Packet&, Payload&);

struct PayloadWelcomeRequest {
};

struct PayloadWelcomeResponse {
    size_t id;
};

#endif // PAYLOADS_HPP_GUARD
