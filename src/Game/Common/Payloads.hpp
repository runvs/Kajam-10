#ifndef PAYLOADS_HPP_GUARD
#define PAYLOADS_HPP_GUARD

#include "KeyCodes.hpp"
#include "Vector.hpp"
#include <SFML/Network.hpp>
#include <cstddef>
#include <map>

struct PayloadClient2Server {

    int playerID;
    std::map<jt::KeyCode, bool> input;
};

sf::Packet& operator<<(sf::Packet&, PayloadClient2Server&);
sf::Packet& operator>>(sf::Packet&, PayloadClient2Server&);

struct PayloadServer2Client {
    int playerID;
    // TODO use real position
    std::map<int, jt::Vector2> playerPositions;
    // TODO Enemy Positions
    // TODO ParticlePositions
};

sf::Packet& operator<<(sf::Packet&, PayloadServer2Client&);
sf::Packet& operator>>(sf::Packet&, PayloadServer2Client&);

#endif // PAYLOADS_HPP_GUARD
