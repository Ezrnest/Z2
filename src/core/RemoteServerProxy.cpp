//
// Created by liyicheng on 2019/7/23.
//
#include "RemoteServerProxy.h"
#include <easylogging++.h>
#include <core/messages/RegisterPlayer.h>

#include "Client.h"
using namespace asio;
using namespace z2;

void z2::RemoteServerProxy::sendMessageToServer(const shared_ptr<z2::Message> &message) {

}

void z2::RemoteServerProxy::handleConnect() {

    //register first
    shared_ptr<Client> cl = client.lock();
    MessagePtr rm(new RegisterPlayer(cl->getPlayerId(),cl->getPlayerName()));
    sendMessageToServer(rm);
    LOG(INFO) << "[RemoteServerProxy] Connection success!";
}

z2::RemoteServerProxy::RemoteServerProxy(const shared_ptr<z2::Client> &client)
        : client(client) {


}

void RemoteServerProxy::handleReceive(const asio::error_code &err, size_t length) {
    if(err){
        LOG(WARNING) << "[RemoteServerProxy] Failed to receive!";
        return;
    }
    std::istream input(&buf);

    MessagePtr msg(dynamic_cast<Message *>(SerializableRegistry::instance().deserialize(input)));
    buf.consume(1); // separator
    if(!msg){
        LOG(WARNING) << "[RemoteServerProxy] Invalid message type!";
        return;
    }
    client.lock()->acceptMessage(msg);

    async_read_until(*socket, buf, '\n',[this](const error_code &error, size_t length){
        handleReceive(error, length);
        return;
    });
}

bool RemoteServerProxy::tryConnect(const string &address, int port) {
    if(running){
        return false;
    }
    ip::tcp::endpoint ep(ip::address::from_string(address), port);
    socket.reset(new ip::tcp::socket(*service));
//    socket->async_connect(ep, [this](const asio::error_code &ec) {
//        handleConnect(ec);
//        return;
//    });
    asio::error_code err;
    socket->connect(ep, err);
    if (err) {
        LOG(WARNING) << "Failed to establish connection, error code = " << err.value();
        return false;
    }
    handleConnect();
    running = true;
    startReceiving();
    return true;
}

void RemoteServerProxy::startReceiving() {
    shared_ptr<RemoteServerProxy> self(this);
    async_read_until(*socket, buf,'\n', [this](const asio::error_code &error, size_t length) { handleReceive(error,length); });

    auto f = [this, self]() {
        service->run();
        LOG(INFO) << "[RemoteServerProxy] Service stopped!";
    };
    workingThread = std::thread(f);

    workingThread.detach();
}


