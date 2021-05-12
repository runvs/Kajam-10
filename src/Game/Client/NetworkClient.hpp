﻿#ifndef NETWORKCLIENT_HPP_GUARD
#define NETWORKCLIENT_HPP_GUARD

#include "Payloads.hpp"
#include <SFML/Network.hpp>
#include <future>
#include <thread>

class NetworkClient {
public:
    NetworkClient(sf::IpAddress address);
    ~NetworkClient();

    bool isNewDataAvailable();
    PayloadServer2Client getData();
    void send(PayloadClient2Server const&);

private:
    sf::IpAddress m_serverAddress;

    std::thread m_thread;
    std::atomic_bool m_stopThread;

    sf::UdpSocket m_socket;

    std::mutex m_dataMutex;
    bool m_newDataReceived;
    PayloadServer2Client m_received_data;

    bool m_newDataToSend;
    PayloadClient2Server m_dataToSend;

    void stopThread();
    void sendDisconnectMessage();
    void startThread();

    void internalReceiveData();
    void internalSendData();
};

#endif
