#include "Conversions.hpp"

sf::Packet& operator<<(sf::Packet& packet, jt::KeyCode& k)
{
    return packet << static_cast<int>(k);
    // << payload.input;
}
sf::Packet& operator<<(sf::Packet& packet, jt::KeyCode const& k)
{
    return packet << static_cast<int>(k);
    // << payload.input;
}

sf::Packet& operator>>(sf::Packet& packet, jt::KeyCode& k)
{
    int ki;
    packet >> ki;
    k = static_cast<jt::KeyCode>(ki);
    return packet;
}

sf::Packet& operator<<(sf::Packet& packet, jt::Vector2& vec)
{
    return packet << vec.x() << vec.y();
}
sf::Packet& operator<<(sf::Packet& packet, jt::Vector2 const& vec)
{
    return packet << vec.x() << vec.y();
}

sf::Packet& operator>>(sf::Packet& packet, jt::Vector2& vec)
{
    return packet >> vec.x() >> vec.y();
}
