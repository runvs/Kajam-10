#ifndef CONNECTION_MANAGER_HPP_GUARD
#define CONNECTION_MANAGER_HPP_GUARD
#include <SFML/Network.hpp>
#include <chrono>
#include <map>
#include <mutex>

struct IP_Endpoint {
    sf::IpAddress address;
    unsigned short port;
};

using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;
struct ConnectionInfo {
    int playerId;
    TimePoint lastReceivedTime;
};

bool operator==(IP_Endpoint const& a, IP_Endpoint const& b);
bool operator<(IP_Endpoint const& a, IP_Endpoint const& b);

class ConnectionManager {
public:
    void updateConnection(sf::IpAddress sender_address, unsigned short sender_port);
    std::vector<IP_Endpoint> getAllActiveConnections();
    void removeInactiveConnections();
    bool isNewConnection(IP_Endpoint con);
    int getPlayerIdForConnection(IP_Endpoint con);
    IP_Endpoint getConnectionForPlayerId(int playerID);
    std::vector<int> getAllPlayerIds();
    void closeConnection(IP_Endpoint const& ip_endpoint);

private:
    using MapType = std::map<IP_Endpoint, ConnectionInfo>;
    MapType m_connections;
    std::mutex m_mutex;
    int currentPlayerId = 0;
};

#endif
