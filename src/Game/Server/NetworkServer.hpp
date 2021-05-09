﻿#ifndef NETWORK_SERVER_HPP_GUARD
#define NETWORK_SERVER_HPP_GUARD

#include "../Common/Payloads.hpp"
#include "ConnectionManager.hpp"

#include <SFML/Network.hpp>
#include <future>
#include <thread>

class NetworkServer {
public:
    NetworkServer();
    ~NetworkServer();

    // TODO access data
    PayloadClient2Server getData();
    // TODO switch to actual data structs
    void send(PayloadServer2Client const&);

private:
    std::thread m_thread;
    std::atomic_bool m_stopThread;

    sf::UdpSocket m_socket;

    ConnectionManager m_connections;

    std::mutex m_dataMutex;
    bool m_newDataReceived;
    PayloadClient2Server m_received_data;

    bool m_newDataToSend;
    PayloadServer2Client m_dataToSend;

    void stopThread();
    void startThread();

    void internalReceiveData();
    void internalSendData();
};

#endif
