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

    std::vector<PayloadClient2Server> getData(int playerId);
    void sendToClient(int playerId, PayloadServer2Client const& payload);
    std::vector<int> getAllPlayerIds();

    void closeConnectionTo(int playerId);

private:
    std::thread m_thread;
    std::atomic_bool m_stopThread;

    sf::UdpSocket m_socket;

    ConnectionManager m_connections;

    std::mutex m_dataMutex;
    std::map<IP_Endpoint, std::vector<PayloadClient2Server>> m_received_data;

    bool m_newDataToSend;
    std::map<IP_Endpoint, PayloadServer2Client> m_dataToSend;
    std::size_t m_messageId;

    void stopThread();
    void startThread();

    void internalReceiveData();
    void internalSendData();
};

#endif
