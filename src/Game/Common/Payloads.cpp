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
    return packet << payload.playerID << payload.playerStates << payload.shots
                  << payload.prediction_id << payload.messageId;
}
sf::Packet& operator>>(sf::Packet& packet, PayloadServer2Client& payload)
{
    return packet >> payload.playerID >> payload.playerStates >> payload.shots
        >> payload.prediction_id >> payload.messageId;
}
