#ifndef PLAYSTATE_GUARD_HPP
#define PLAYSTATE_GUARD_HPP

#include "SFML/Network/Packet.hpp"
#include "Vector.hpp"

#include <map>

struct PlayerState {
    jt::Vector2 position;
};

using PlayerMap = std::map<int, PlayerState>;

sf::Packet& operator<<(sf::Packet&, PlayerState&);
sf::Packet& operator<<(sf::Packet&, PlayerState const&);
sf::Packet& operator>>(sf::Packet&, PlayerState&);

#endif
