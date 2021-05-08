#ifndef CONNECTION_MANAGER_HPP_GUARD
#define CONNECTION_MANAGER_HPP_GUARD
#include <SFML/Network.hpp>
#include <chrono>

struct Connection {
    sf::IpAddress address;
    unsigned short port;
};
bool operator==(Connection const& a, Connection const& b);
bool operator<(Connection const& a, Connection const& b);

class ConnectionManager {
public:
    void addIfNewConnection(sf::IpAddress sender_address, unsigned short sender_port);
    std::vector<Connection> getConnections();
    void update();

private:
    using MapType = std::map<Connection, std::chrono::time_point<std::chrono::system_clock>>;
    MapType m_connections;
    int m_currentNewConnectionId = 0;

    bool isNewConnection(Connection con);
};

#endif
