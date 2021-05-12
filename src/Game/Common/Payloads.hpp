#ifndef PAYLOADS_HPP_GUARD
#define PAYLOADS_HPP_GUARD

#include "InputState.hpp"
#include "PlayerState.hpp"
#include <SFML/Network.hpp>
#include <map>

struct PayloadClient2Server {

    int playerID;
    InputState input;
    float dt;
    std::size_t currentPredictionId;
};

sf::Packet& operator<<(sf::Packet&, PayloadClient2Server&);
sf::Packet& operator>>(sf::Packet&, PayloadClient2Server&);

struct PayloadServer2Client {
    int playerID;
    PlayerMap playerStates;
    std::size_t prediction_id;

    // TODO Enemy Positions
    // TODO ParticlePositions
};

sf::Packet& operator<<(sf::Packet&, PayloadServer2Client&);
sf::Packet& operator>>(sf::Packet&, PayloadServer2Client&);

#endif // PAYLOADS_HPP_GUARD
