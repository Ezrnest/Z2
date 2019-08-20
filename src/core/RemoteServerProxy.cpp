//
// Created by liyicheng on 2019/7/23.
//
#include "RemoteServerProxy.h"
#include "plog/Log.h"
#include <core/messages/RegisterPlayer.h>
#include <core/messages/SignalMessage.h>

#include "Client.h"
#include "MessageConductor.h"

using namespace asio;
using namespace z2;

void z2::RemoteServerProxy::sendMessageToServer(const shared_ptr<z2::Message> &message) {
    if(socket){
        auto ec = MessageConductor::sendMessageToSocket(socket, message);
        if(ec){
            PLOG_WARNING << "[RemoteServerProxy] Failed to send message, error code = " << ec.value();
            if(!client.expired()){
                client.lock()->onConnectionLost();
            }
        }
    }
}

bool z2::RemoteServerProxy::handleConnect() {

    //register first
    shared_ptr<Client> cl = client.lock();
    MessagePtr rm(new RegisterPlayer(cl->getPlayerId(), cl->getPlayerName()));
    sendMessageToServer(rm);
    if (!waitForGoodMessage()) {
        return false;
    }
    PLOG(plog::info) << "[RemoteServerProxy] Connection success!";
    return true;
}

z2::RemoteServerProxy::RemoteServerProxy(const shared_ptr<z2::Client> &client)
        : client(client) {

}

inline MessagePtr readMessageFromBuf(asio::streambuf &buf) {
    std::istream input(&buf);
    MessagePtr msg(dynamic_cast<Message *>(SerializableRegistry::instance().deserialize(input)));
    string s;
    std::getline(input, s);
    return msg;
}

void RemoteServerProxy::handleReceive(const asio::error_code &err, size_t length) {
    if (err) {
        PLOG_WARNING << "[RemoteServerProxy] Failed to receive!";
//        stop();
        if(!client.expired()){
            client.lock()->onConnectionLost();
        }
        return;
    }
    MessagePtr msg = readMessageFromBuf(buf);
    if (!msg) {
        PLOG_WARNING << "[RemoteServerProxy] Invalid message type!";
        return;
    }
    PLOG_INFO << "[RemoteServerProxy] Receiving message: " << msg->getClassName();
    if(!client.expired()){
        client.lock()->acceptMessage(msg);
        async_read_until(*socket, buf, '\n', [this](const error_code &error, size_t length) {
            handleReceive(error, length);
       });
    }
}

bool RemoteServerProxy::waitForGoodMessage() {
    asio::error_code err;
    read_until(*socket, buf, '\n', err);
    if (err) {
        PLOG_WARNING << "[RemoteServerProxy] Failed to wait for good!";
        return false;
    }
//    auto temp = buf.data();
//    std::string result(asio::buffers_begin(temp), buffers_begin(temp) + buf.size());
//    PLOG_INFO << "Received data: [" << result << ']';

    shared_ptr<SignalMessage> msg = dynamic_pointer_cast<SignalMessage>(readMessageFromBuf(buf));
    if (!msg || (msg->getCode() != SignalMessage::GOOD)) {
        PLOG_WARNING << "[RemoteServerProxy] Failed to wait for good!";
        return false;
    }
    return true;
}

bool RemoteServerProxy::tryConnectToAddress(const shared_ptr<RemoteServerProxy> &self, const asio::ip::address &address,
                                            int port) {
    ip::tcp::endpoint ep(address, port);
    socket.reset(new ip::tcp::socket(service));
    asio::error_code err;
    socket->connect(ep, err);
    if (err) {
        socket->close();
        return false;
    }
    PLOG_INFO << "[RemoteServerProxy] Connected to " << address.to_string();
    return true;
}



bool RemoteServerProxy::tryConnect0(const shared_ptr<RemoteServerProxy> &self, const string &address, int port) {
    if (running) {
        return false;
    }
    bool success = false;
    try{
        success = tryConnectToAddress(self,ip::address::from_string(address),port);
    }catch(...){
        success = false;
    }
    if(!success){
        ip::tcp::resolver resolver(service);
        ip::tcp::resolver::query query(address, "");
        ip::tcp::resolver::iterator iter = resolver.resolve(query);
        ip::tcp::resolver::iterator end; // End marker.
        while(iter != end){
            ip::tcp::endpoint ep = *iter++;
            if(tryConnectToAddress(self,ep.address(),port)){
                success = true;
                break;
            }
        }
    }
    if(!success){
        PLOG_WARNING << "[RemoteServerProxy] Failed to establish connection.";
        return false;
    }

    if (!handleConnect()) {
        return false;
    }
    running = true;
    startReceiving(self);
    return true;
}

void RemoteServerProxy::startReceiving(const shared_ptr<RemoteServerProxy> &self) {
    async_read_until(*socket, buf, '\n',
                     [this](const asio::error_code &error, size_t length) { handleReceive(error, length); });

    auto f = [this, self]() {
        PLOG(plog::info) << "[RemoteServerProxy] Service started!";
        service.run();
        PLOG(plog::info) << "[RemoteServerProxy] Service stopped!";
    };
    workingThread = std::thread(f);
    workingThread.detach();
}

bool RemoteServerProxy::tryConnect(const shared_ptr<RemoteServerProxy> &self, const string &address, int port) {
    return self->tryConnect0(self, address, port);
}

void RemoteServerProxy::stop() {
    if(socket){
        socket->close();
        socket.reset();
        PLOG_INFO << "[RemoteServerProxy] Socket reset";
    }
    if(!service.stopped()){
        service.stop();
    }

    if(workingThread.joinable()){
        workingThread.join();
    }
}

RemoteServerProxy::~RemoteServerProxy() {
    stop();
}


