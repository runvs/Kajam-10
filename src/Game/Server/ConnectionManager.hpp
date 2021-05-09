#ifndef CONNECTION_MANAGER_HPP_GUARD
#define CONNECTION_MANAGER_HPP_GUARD
#include <SFML/Network.hpp>
#include <chrono>
#include <map>
#include <mutex>

struct Connection {
    sf::IpAddress address;
    unsigned short port;
};
using TimePoint = std::chrono::time_point<std::chrono::system_clock>;
struct ConnectionInfo {
    int playerId;
    TimePoint lastReceivedTime;
};

bool operator==(Connection const& a, Connection const& b);
bool operator<(Connection const& a, Connection const& b);

class ConnectionManager {
public:
    void updateConnection(sf::IpAddress sender_address, unsigned short sender_port);
    std::vector<Connection> getAllActiveConnections();
    void removeInactiveConnections();
    bool isNewConnection(Connection con);
    int getPlayerIdForConnection(Connection con);
    Connection getConnectionForPlayerId(int playerID);

private:
    using MapType = std::map<Connection, ConnectionInfo>;
    MapType m_connections;
    std::mutex m_mutex;
    int currentPlayerId = 0;
};

#endif
