#include "NetworkServer.hpp"
#include "common.hpp"
#include <iostream>

NetworkServer::NetworkServer()
{
    m_socket.bind(Network::NetworkProperties::port());
    m_socket.setBlocking(false);

    m_stopThread.store(false);
    m_newDataReceived = false;
    m_newDataToSend = false;

    startThread();
}

void NetworkServer::stopThread()
{
    m_stopThread.store(true);
    m_thread.join();
}

void NetworkServer::startThread()
{
    m_thread = std::thread([this]() {
        while (true) {
            // exit thread
            if (m_stopThread.load()) {
                std::cout << "kill thread" << std::endl;
                break;
            }

            internalReceiveData();
            internalSendData();
            m_connections.removeInactiveConnections();
        }
    });
}

NetworkServer::~NetworkServer() { stopThread(); }

std::vector<PayloadClient2Server> NetworkServer::getData(int playerId)
{
    std::lock_guard const lock { m_dataMutex };
    auto data = std::vector<PayloadClient2Server>();
    ;
    if (m_newDataReceived) {
        m_newDataReceived = false;
        auto const con = m_connections.getConnectionForPlayerId(playerId);
        data = m_received_data[con];
        m_received_data[con].clear();
    }
    return data;
}

void NetworkServer::sendToClient(int playerId, PayloadServer2Client const& payload)
{
    std::lock_guard const lock { m_dataMutex };
    m_dataToSend[m_connections.getConnectionForPlayerId(playerId)] = payload;
    m_newDataToSend = true;
}

void NetworkServer::internalReceiveData()
{
    sf::Packet packet;
    sf::IpAddress sender_address;
    unsigned short sender_port;

    auto const status = m_socket.receive(packet, sender_address, sender_port);

    if (status == sf::Socket::Status::NotReady) { /*nothing received, try again in next iteration*/
        // std::cout << "not ready\n";

    } else if (status == sf::Socket::Status::Done) {
        // std::cout << "received data\n";
        std::lock_guard const lockData { m_dataMutex };
        m_newDataReceived = true;
        IP_Endpoint const con { sender_address, sender_port };

        PayloadClient2Server payload;
        packet >> payload;
        m_received_data[con].emplace_back(std::move(payload));

        // TODO if new connection, send back "welcome" packet

        m_connections.updateConnection(sender_address, sender_port);

    } else if (status == sf::Socket::Status::Error) {
        std::cout << "network error\n";
    }
}

void NetworkServer::internalSendData()
{
    std::unique_lock lock { m_dataMutex };
    if (m_newDataToSend) {
        for (auto& kvp : m_dataToSend) {
            sf::Packet packet;
            packet << kvp.second;
            m_socket.send(packet, kvp.first.address, kvp.first.port);
        }

        m_dataToSend.clear();
        m_newDataToSend = false;
    }
}

std::vector<int> NetworkServer::getAllPlayerIds() { return m_connections.getAllPlayerIds(); }
