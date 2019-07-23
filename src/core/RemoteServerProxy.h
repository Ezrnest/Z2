//
// Created by liyicheng on 2019/7/23.
//

#ifndef Z2_REMOTESERVERPROXY_H
#define Z2_REMOTESERVERPROXY_H

#include "ServerProxy.h"

#define ASIO_STANDALONE
#include "asio.hpp"

#include <memory>
#include <thread>

namespace z2 {
using SocketPtr = shared_ptr<asio::ip::tcp::socket>;
class Client;
class RemoteServerProxy : public ServerProxy{
private:
    SocketPtr socket;

    weak_ptr<Client> client;

    shared_ptr<asio::io_service> service;

    asio::streambuf buf;

    std::thread workingThread;

    bool running = false;

    /**
     * Do basic
     */
    void handleConnect();

    void handleReceive(const asio::error_code &err, size_t length);




    void startReceiving();

public:
    /**
     * Creates the server proxy and tries to connect to the server.
     */
    RemoteServerProxy(const shared_ptr<Client>& client);


    /**
     * Tries to connect to the given address. The player will be registered.
     *
     * Returns `true` if the connection is established.
     */
    bool tryConnect(const string &address, int port);

    void sendMessageToServer(const MessagePtr &message) override;


};

}
#endif //Z2_REMOTESERVERPROXY_H
