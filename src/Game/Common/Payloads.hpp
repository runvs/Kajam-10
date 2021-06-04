#ifndef PAYLOADS_HPP_GUARD
#define PAYLOADS_HPP_GUARD

#include "EnemyAI.hpp"
#include "EnemyState.hpp"
#include "ExplosionState.hpp"
#include "InputState.hpp"
#include "PlayerState.hpp"
#include "PowerupState.hpp"
#include "ShotState.hpp"
#include <SFML/Network.hpp>
#include <map>

struct PayloadClient2Server {

    int playerID;
    InputState input;
    float dt;
    std::size_t currentPredictionId;
    bool disconnect;
    std::size_t messageId;
};

sf::Packet& operator<<(sf::Packet&, PayloadClient2Server&);
sf::Packet& operator>>(sf::Packet&, PayloadClient2Server&);

struct PayloadServer2Client {

    int playerID { -1 };
    PlayerMap playerStates {};
    std::size_t predictionId { 0 };
    std::size_t messageId { 0 };

    std::vector<ShotState> shots {};
    std::vector<EnemyState> enemies {};
    std::vector<PowerupState> powerups {};
    std::vector<ExplosionState> explosions {};
    bool shotFired { false };

    int score { 0 };
    int level { 0 };
    bool levelChanged { false };
};

sf::Packet& operator<<(sf::Packet&, PayloadServer2Client&);
sf::Packet& operator>>(sf::Packet&, PayloadServer2Client&);

#endif // PAYLOADS_HPP_GUARD
