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
using SocketPtr = asio::ip::tcp::socket*;

class Client;

class RemoteServerProxy : public ServerProxy {
private:
    

    weak_ptr<Client> client;

    asio::io_service service;

    SocketPtr socket;
    
    asio::streambuf buf;

    std::thread workingThread;

    bool running = false;

    /**
     * Do basic
     */
    bool handleConnect();

    bool waitForGoodMessage();

    void handleReceive(const asio::error_code &err, size_t length);


    void startReceiving(const shared_ptr<RemoteServerProxy> &self);

    bool tryConnect0(const shared_ptr<RemoteServerProxy> &self, const string &address, int port);

public:
    /**
     * Creates the server proxy and tries to connect to the server.
     */
    explicit RemoteServerProxy(const shared_ptr<Client> &client);


    void sendMessageToServer(const MessagePtr &message) override;

    /**
     * Requires the proxy to stop receiving messages.
     */
    void stop();

    /**
     * Tries to connect to the given address. The player will be registered.
     *
     * Returns `true` if the connection is established.
     */
    static bool tryConnect(const shared_ptr<RemoteServerProxy> &self, const string &address, int port);
};

}
#endif //Z2_REMOTESERVERPROXY_H
