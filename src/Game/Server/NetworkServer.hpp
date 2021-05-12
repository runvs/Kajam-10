#ifndef NETWORK_SERVER_HPP_GUARD
#define NETWORK_SERVER_HPP_GUARD

#include "ConnectionManager.hpp"
#include "Payloads.hpp"
#include <SFML/Network.hpp>
#include <future>
#include <thread>

class NetworkServer {
public:
    NetworkServer();
    ~NetworkServer();

    // TODO with the vector, the bool is no longer needed.
    std::vector<PayloadClient2Server> getData(int playerId);
    // TODO switch to actual data structs
    void send(PayloadServer2Client const&);
    std::vector<int> getAllPlayerIds();

private:
    std::thread m_thread;
    std::atomic_bool m_stopThread;

    sf::UdpSocket m_socket;

    ConnectionManager m_connections;

    std::mutex m_dataMutex;
    bool m_newDataReceived;
    std::map<IP_Endpoint, std::vector<PayloadClient2Server>> m_received_data;

    bool m_newDataToSend;
    PayloadServer2Client m_dataToSend;

    void stopThread();
    void startThread();

    void internalReceiveData();
    void internalSendData();
};

#endif
