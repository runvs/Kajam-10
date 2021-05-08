#include "NetworkServer.hpp"
#include "../Common/common.hpp"

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
            m_connections.update();
        }
    });
}

NetworkServer::~NetworkServer() { stopThread(); }

void NetworkServer::getData()
{
    std::lock_guard const lock { m_dataMutex };
    if (m_newDataReceived) {

        m_newDataReceived = false;
    }
}

void NetworkServer::send(std::string const& message)
{
    std::lock_guard const lock { m_dataMutex };
    m_newDataToSend = true;
    m_dataToSend = message;
}

void NetworkServer::internalReceiveData()
{
    sf::Packet packet;
    sf::IpAddress sender_address;
    unsigned short sender_port;

    auto const status = m_socket.receive(packet, sender_address, sender_port);

    if (status == sf::Socket::Status::NotReady) { /*nothing received, try again later*/
        // std::cout << "not ready\n";

    } else if (status == sf::Socket::Status::Done) {
        // std::cout << "received data\n";
        std::lock_guard const lockData { m_dataMutex };
        m_newDataReceived = true;
        m_connections.addIfNewConnection(sender_address, sender_port);
    } else if (status == sf::Socket::Status::Error) {
        std::cout << "network error\n";
    }
}

void NetworkServer::internalSendData()
{
    std::unique_lock lock { m_dataMutex };
    if (m_newDataToSend) {
        auto packet = Network::Packets::serializeTestPacket(1, m_dataToSend);
        lock.unlock();

        for (auto con : m_connections.getConnections()) {
            std::cout << "send socket data to connection: " << con.address.toString() << ":"
                      << con.port << std::endl;
            if (m_socket.send(packet, con.address, con.port) != sf::Socket::Status::Done) {
                std::cout << "error sending data\n";
            }
        }
        m_newDataToSend = false;
    }
}
