#include "ConnectionManager.hpp"
#include "SystemHelper.hpp"
#include <iostream>

bool operator==(Connection const& a, Connection const& b)
{
    return a.port == b.port && a.address == b.address;
}

bool operator<(Connection const& a, Connection const& b)
{
    return a.port < b.port && a.address.toInteger() < b.address.toInteger();
}

void ConnectionManager::addIfNewConnection(sf::IpAddress sender_address, unsigned short sender_port)
{
    Connection con { sender_address, sender_port };
    if (isNewConnection(con)) {
        std::cout << "add new connection: " << sender_address.toString() << ":" << sender_port
                  << std::endl;
        m_connections[con] = std::chrono::system_clock::now();
    }
}

bool ConnectionManager::isNewConnection(Connection newCon)
{
    bool const alreadyPresent = std::any_of(m_connections.begin(), m_connections.end(),
        [&newCon](auto kvp) { return kvp.first == newCon; });
    return !alreadyPresent;
}

void ConnectionManager::update()
{
    auto now = std::chrono::system_clock::now();
    jt::SystemHelper::erase_if(m_connections, [](auto const /**/) { return false; });
}

std::vector<Connection> ConnectionManager::getConnections()
{
    std::vector<Connection> connections;
    std::transform(m_connections.begin(), m_connections.end(), std::back_inserter(connections),
        [](auto kvp) { return kvp.first; });

    return connections;
}
