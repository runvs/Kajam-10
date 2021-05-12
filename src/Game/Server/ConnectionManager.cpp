#include "ConnectionManager.hpp"
#include "SystemHelper.hpp"
#include "common.hpp"
#include <iostream>

bool operator==(IP_Endpoint const& a, IP_Endpoint const& b)
{
    return a.port == b.port && a.address == b.address;
}

bool operator<(IP_Endpoint const& a, IP_Endpoint const& b)
{
    return a.port < b.port && a.address.toInteger() < b.address.toInteger();
}

void ConnectionManager::updateConnection(sf::IpAddress sender_address, unsigned short sender_port)
{
    IP_Endpoint const con { sender_address, sender_port };
    if (isNewConnection(con)) {
        std::cout << "add new connection: " << sender_address.toString() << ":" << sender_port
                  << std::endl;
        std::lock_guard lock { m_mutex };
        m_connections[con] = ConnectionInfo { currentPlayerId++, std::chrono::steady_clock::now() };
        return;
    }
    std::lock_guard lock { m_mutex };
    m_connections[con]
        = ConnectionInfo { getPlayerIdForConnection(con), std::chrono::steady_clock::now() };
}

int ConnectionManager::getPlayerIdForConnection(IP_Endpoint con)
{
    int playerId = -1;
    if (m_connections.count(con)) {
        return m_connections[con].playerId;
    }
    return -1;
}

IP_Endpoint ConnectionManager::getConnectionForPlayerId(int playerID)
{
    for (auto kvp : m_connections) {
        if (kvp.second.playerId == playerID)
            return kvp.first;
    }
    return IP_Endpoint {};
}

std::vector<int> ConnectionManager::getAllPlayerIds()
{
    std::vector<int> out;
    std::transform(m_connections.begin(), m_connections.end(), std::back_inserter(out),
        [](auto kvp) { return kvp.second.playerId; });
    return out;
}

bool ConnectionManager::isNewConnection(IP_Endpoint newCon)
{
    std::lock_guard lock { m_mutex };
    bool const alreadyPresent = std::any_of(m_connections.begin(), m_connections.end(),
        [&newCon](auto kvp) { return kvp.first == newCon; });
    return !alreadyPresent;
}

void ConnectionManager::removeInactiveConnections()
{
    auto now = std::chrono::steady_clock::now();
    std::lock_guard lock { m_mutex };
    jt::SystemHelper::erase_if(m_connections, [&now](auto const kvp) {
        float const elapsed_in_seconds = std::chrono::duration_cast<std::chrono::microseconds>(
                                             now - kvp.second.lastReceivedTime)
                                             .count()
            / 1000.0f / 1000.0f;
        if (elapsed_in_seconds >= Network::NetworkProperties::serverTimeToClientTimeout()) {
            std::cout << "remove connection after timeout\n";
            return true;
        }
        return false;
    });
}

std::vector<IP_Endpoint> ConnectionManager::getAllActiveConnections()
{
    std::vector<IP_Endpoint> connections;
    std::lock_guard lock { m_mutex };
    std::transform(m_connections.begin(), m_connections.end(), std::back_inserter(connections),
        [](auto kvp) { return kvp.first; });

    return connections;
}
