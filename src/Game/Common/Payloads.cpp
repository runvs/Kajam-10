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
    packet << payload.prediction_id;
    packet << payload.messageId;
    packet << payload.score;

    return packet;
}

sf::Packet& operator>>(sf::Packet& packet, PayloadServer2Client& payload)
{
    packet >> payload.playerID;
    packet >> payload.playerStates;
    packet >> payload.shots;
    packet >> payload.enemies;
    packet >> payload.prediction_id;
    packet >> payload.messageId;
    packet >> payload.score;

    return packet;
}
