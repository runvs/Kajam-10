#ifndef NETWORKCLIENT_HPP_GUARD
#define NETWORKCLIENT_HPP_GUARD

#include <SFML/Network.hpp>
#include <future>
#include <thread>

#include "../Common/Payloads.hpp"

class NetworkClient {
public:
    NetworkClient(sf::IpAddress address);
    ~NetworkClient();

    bool isNewDataAvailable();
    // TODO access data
    Payload getData();
    // TODO switch to actual data structs
    void send(Payload const&);

private:
    sf::IpAddress m_serverAddress;

    std::thread m_thread;
    std::atomic_bool m_stopThread;

    sf::UdpSocket m_socket;

    std::mutex m_dataMutex;
    bool m_newDataReceived;
    Payload m_received_data;

    bool m_newDataToSend;
    Payload m_dataToSend;

    void stopThread();
    void startThread();

    void internalReceiveData();
    void internalSendData();
};

#endif
