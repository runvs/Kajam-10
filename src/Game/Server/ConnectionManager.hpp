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
bool operator==(Connection const& a, Connection const& b);
bool operator<(Connection const& a, Connection const& b);

class ConnectionManager {
public:
    void updateConnection(sf::IpAddress sender_address, unsigned short sender_port);
    std::vector<Connection> getAllActiveConnections();
    void removeInactiveConnections();
    bool isNewConnection(Connection con);

private:
    using MapType = std::map<Connection, std::chrono::time_point<std::chrono::system_clock>>;
    MapType m_connections;
    std::mutex m_mutex;
};

#endif
