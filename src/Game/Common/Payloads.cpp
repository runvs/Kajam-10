#include "Payloads.hpp"
#include "Conversions.hpp"

sf::Packet& operator<<(sf::Packet& packet, PayloadClient2Server& payload)
{
    packet << payload.playerID;
    packet << payload.input;
    packet << payload.dt;
    packet << payload.currentPredictionId;
    packet << payload.disconnect;
    packet << payload.messageId;

    return packet;
}
sf::Packet& operator>>(sf::Packet& packet, PayloadClient2Server& payload)
{
    packet >> payload.playerID;
    packet >> payload.input;
    packet >> payload.dt;
    packet >> payload.currentPredictionId;
    packet >> payload.disconnect;
    packet >> payload.messageId;

    return packet;
}

sf::Packet& operator<<(sf::Packet& packet, PayloadServer2Client& payload)
{
    packet << payload.playerID;
    packet << payload.playerStates;
    packet << payload.shots;
    packet << payload.enemies;
    packet << payload.powerups;
    packet << payload.explosions;
    packet << payload.predictionId;
    packet << payload.messageId;
    packet << payload.score;
    packet << payload.level;
    packet << payload.shotFired;
    return packet;
}

sf::Packet& operator>>(sf::Packet& packet, PayloadServer2Client& payload)
{
    packet >> payload.playerID;
    packet >> payload.playerStates;
    packet >> payload.shots;
    packet >> payload.enemies;
    packet >> payload.powerups;
    packet >> payload.explosions;
    packet >> payload.predictionId;
    packet >> payload.messageId;
    packet >> payload.score;
    packet >> payload.level;
    packet >> payload.shotFired;
    return packet;
}
