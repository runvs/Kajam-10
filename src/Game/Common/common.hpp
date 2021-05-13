#ifndef COMMON_GUARD_HPP
#define COMMON_GUARD_HPP

#include "Config.hpp"
#include <cstddef>
#include <sfml/Network.hpp>

namespace {
auto configSettings = ConfigSettings("config.json");
}

namespace Network {
struct NetworkProperties {
    static std::string serverIp() { return configSettings.serverIp; }
    static unsigned short port() { return configSettings.serverPort; }
    static float serverTimeToClientTimeout() { return 2.0f; }
    static float serverTickTime() { return 0.01f; }

    static constexpr std::size_t clientNetworkBufferSize() { return (2 << 9); }
    /*
     * Trick: Instead of
     *  index = current_id % size;
     * it is possible to calculate
     *  index = current_id & (size - 1)
     * if size is a power of 2
     */
    static constexpr std::size_t clientNetworkBufferMask() { return clientNetworkBufferSize() - 1; }
};

} // namespace Network

namespace Game {
struct GameProperties {
    static float playerMovementSpeed() { return 100.0f; }
    static float playerMaxAllowedPredictionError() { return 4.0f * 4.0f; };
};
} // namespace Game

#endif
