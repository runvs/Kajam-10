﻿#include "NetworkClient.hpp"
#include "../Common/common.hpp"

#include <iostream>

NetworkClient::NetworkClient(sf::IpAddress address)
{
    m_serverAddress = address;

    m_socket.bind(Network::NetworkProperties::port() - 1);
    m_socket.setBlocking(false);

    m_stopThread.store(false);
    m_newDataReceived = false;
    m_newDataToSend = false;

    startThread();
}

void NetworkClient::stopThread()
{
    m_stopThread.store(true);
    m_thread.join();
}

void NetworkClient::startThread()
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
        }
    });
}

NetworkClient::~NetworkClient() { stopThread(); }

bool NetworkClient::isNewDataAvailable()
{
    std::lock_guard const lock { m_dataMutex };
    return m_newDataReceived;
}

Payload NetworkClient::getData()
{
    std::lock_guard const lock { m_dataMutex };
    if (m_newDataReceived) {
        m_newDataReceived = false;
        return m_received_data;
    }
    return Payload { 0, "" };
}

void NetworkClient::send(Payload const& payload)
{
    std::lock_guard const lock { m_dataMutex };
    m_newDataToSend = true;
    m_dataToSend = payload;
}

void NetworkClient::internalReceiveData()
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
        packet >> m_received_data;
    } else if (status == sf::Socket::Status::Error) {
        std::cout << "network error\n";
    }
}

void NetworkClient::internalSendData()
{
    std::unique_lock lock { m_dataMutex };
    if (m_newDataToSend) {
        sf::Packet packet;
        packet << m_dataToSend;
        lock.unlock();

        if (m_socket.send(packet, m_serverAddress, Network::NetworkProperties::port())
            != sf::Socket::Status::Done) {
            std::cout << "error sending data\n";
        }

        m_newDataToSend = false;
    }
}