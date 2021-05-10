#include "Payloads.hpp"

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

template <typename T>
sf::Packet& operator<<(sf::Packet& packet, std::vector<T>& vec)
{
    packet << vec.size();
    for (auto const& i : vec) {
        packet << i;
    }
    return packet;
}

template <typename T>
sf::Packet& operator>>(sf::Packet& packet, std::vector<T>& vec)
{
    std::size_t size;
    packet >> size;

    for (std::size_t i = 0; i != size; ++i) {
        T obj;
        packet >> obj;
        vec.emplace_back(obj);
    }
    return packet;
}

template <typename K, typename V>
sf::Packet& operator<<(sf::Packet& packet, std::map<K, V>& map)
{
    packet << map.size();
    for (auto const& kvp : map) {
        packet << kvp.first << kvp.second;
    }
    return packet;
}

template <typename K, typename V>
sf::Packet& operator>>(sf::Packet& packet, std::map<K, V>& map)
{
    std::size_t size;
    packet >> size;

    for (std::size_t i = 0; i != size; ++i) {
        K key;
        V value;
        packet >> key >> value;
        map[key] = value;
    }
    return packet;
}

sf::Packet& operator<<(sf::Packet& packet, PayloadClient2Server& payload)
{
    return packet << payload.playerID << payload.input;
}

sf::Packet& operator>>(sf::Packet& packet, PayloadClient2Server& payload)
{
    return packet >> payload.playerID >> payload.input;
}

sf::Packet& operator<<(sf::Packet& packet, PayloadServer2Client& payload)
{
    return packet << payload.playerID << payload.playerPositions;
}
sf::Packet& operator>>(sf::Packet& packet, PayloadServer2Client& payload)
{
    return packet >> payload.playerID >> payload.playerPositions;
}
