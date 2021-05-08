#ifndef NETWORKCLIENT_HPP_GUARD
#define NETWORKCLIENT_HPP_GUARD

#include <SFML/Network.hpp>
#include <future>
#include <thread>

class NetworkClient {
public:
    NetworkClient(sf::IpAddress address);
    ~NetworkClient();

    // TODO access data
    void getData();
    // TODO switch to actual data structs
    void send(std::string const& message);

private:
    sf::IpAddress m_serverAddress;

    std::thread m_thread;
    std::atomic_bool m_stopThread;

    sf::UdpSocket m_socket;

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