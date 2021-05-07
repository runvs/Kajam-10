#ifndef COMMON_GUARD_HPP
#define COMMON_GUARD_HPP

#include <cstddef>
#include <sfml/Network.hpp>
#include <string>

namespace Network {
class NetworkProperties {
public:
    static unsigned short port() { return 53000; }
};

namespace Packets {
sf::Packet serializeTestPacket(std::size_t, std::string);
void deserializeTestPacket(sf::Packet, std::size_t&, std::string&);
} // namespace Packets

} // namespace Network

#endif
