﻿#include "Payloads.hpp"
#include "Conversions.hpp"

sf::Packet& operator<<(sf::Packet& packet, PayloadClient2Server& payload)
{
    return packet << payload.playerID << payload.input << payload.dt << payload.currentPredictionId
                  << payload.disconnect;
}

sf::Packet& operator>>(sf::Packet& packet, PayloadClient2Server& payload)
{
    return packet >> payload.playerID >> payload.input >> payload.dt >> payload.currentPredictionId
        >> payload.disconnect;
}

sf::Packet& operator<<(sf::Packet& packet, PayloadServer2Client& payload)
{
    return packet << payload.playerID << payload.playerStates << payload.prediction_id;
}
sf::Packet& operator>>(sf::Packet& packet, PayloadServer2Client& payload)
{
    return packet >> payload.playerID >> payload.playerStates >> payload.prediction_id;
}
