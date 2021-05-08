#ifndef NETWORK_SERVER_HPP_GUARD
#define NETWORK_SERVER_HPP_GUARD

#include "ConnectionManager.hpp"

#include <SFML/Network.hpp>
#include <future>
#include <thread>

class NetworkServer {
public:
    NetworkServer();
    ~NetworkServer();

    // TODO access data
    void getData();
    // TODO switch to actual data structs
    void send(std::string const& message);

private:
    std::thread m_thread;
    std::atomic_bool m_stopThread;

    sf::UdpSocket m_socket;

    ConnectionManager m_connections;

    std::mutex m_dataMutex;
    bool m_newDataReceived;

    bool m_newDataToSend;
    std::string m_dataToSend;

    void stopThread();
    void startThread();

    void internalReceiveData();
    void internalSendData();
};

#endif
