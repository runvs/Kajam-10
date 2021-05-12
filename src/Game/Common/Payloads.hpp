#ifndef PAYLOADS_HPP_GUARD
#define PAYLOADS_HPP_GUARD

#include "KeyCodes.hpp"
#include "PlayerState.hpp"
#include <SFML/Network.hpp>
#include <map>

struct PayloadClient2Server {

    int playerID;
    std::map<jt::KeyCode, bool> input;
};

sf::Packet& operator<<(sf::Packet&, PayloadClient2Server&);
sf::Packet& operator>>(sf::Packet&, PayloadClient2Server&);

struct PayloadServer2Client {
    int playerID;
    PlayerMap playerStates;
    // TODO Enemy Positions
    // TODO ParticlePositions
};

sf::Packet& operator<<(sf::Packet&, PayloadServer2Client&);
sf::Packet& operator>>(sf::Packet&, PayloadServer2Client&);

#endif // PAYLOADS_HPP_GUARD
