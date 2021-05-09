#ifndef PAYLOADS_HPP_GUARD
#define PAYLOADS_HPP_GUARD

#include "Vector.hpp"
#include <SFML/Network.hpp>
#include <cstddef>
#include <map>

struct PayloadClient2Server {

    std::size_t playerID;
    // TODO Real input data
    std::map<int, int> input;
};

sf::Packet& operator<<(sf::Packet&, PayloadClient2Server&);
sf::Packet& operator>>(sf::Packet&, PayloadClient2Server&);

struct PayloadServer2Client {
    // TODO use real position
    std::map<std::size_t, int> playerPositions;
    // TODO Enemy Positions
    // TODO ParticlePositions
};

sf::Packet& operator<<(sf::Packet&, PayloadServer2Client&);
sf::Packet& operator>>(sf::Packet&, PayloadServer2Client&);

#endif // PAYLOADS_HPP_GUARD
