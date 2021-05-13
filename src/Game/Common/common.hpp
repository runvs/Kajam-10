#ifndef COMMON_GUARD_HPP
#define COMMON_GUARD_HPP

#include "Config.hpp"
#include <cstddef>

namespace {
ConfigSettings configSettings("config.json");
}

namespace Network {
struct NetworkProperties {
    static std::string serverIp() { return configSettings.serverIp; }
    static unsigned short serverPort() { return configSettings.serverPort; }
    static unsigned short clientPort() { return configSettings.clientPort; }
    static float serverTimeToClientTimeout() { return 2.0f; }
    static float serverTickTime() { return 0.01f; }

    static constexpr std::size_t clientNetworkBufferSize() { return (2 << 9); }
    static size_t serverMaxConnections() { return 8; }
};

} // namespace Network

namespace Game {
struct GameProperties {
    static float playerMovementSpeed() { return 100.0f; }
    static float playerMaxAllowedPredictionError() { return 4.0f * 4.0f; }
    static float shotVelocity() { return 200.0f; }
    static float shotLifeTime() { return 5.0f; }
    static float playerShootCooldown() { return 0.3f; }
};
} // namespace Game

#endif
