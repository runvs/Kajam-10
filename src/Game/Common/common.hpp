#ifndef COMMON_GUARD_HPP
#define COMMON_GUARD_HPP

#include "Config.hpp"
#include <cstddef>
#include <sfml/Network.hpp>
#include <string>

namespace {
auto configSettings = ConfigSettings("config.json");
}

namespace Network {
struct NetworkProperties {
    static std::string serverIp() { return configSettings.serverIp; }
    static unsigned short port() { return configSettings.serverPort; }
    static float serverTimeToClientTimeout() { return 2.0f; }
    static constexpr std::size_t c_buffer_size() { return 512; }
    static constexpr std::size_t c_buffer_mask() { return c_buffer_size() - 1; }
};

namespace Packets {
sf::Packet serializeTestPacket(std::size_t, std::string);
void deserializeTestPacket(sf::Packet, std::size_t&, std::string&);
} // namespace Packets

} // namespace Network

namespace Game {
struct GameProperties {
    static float PlayerMovementSpeed() { return 100.0f; }
    static float PlayerMaxAllowedPredictionError() { return 1.0f; };
};
} // namespace Game

#endif
