#include <memory>

#include <utility>
#include <sstream>

//
// Created by liyicheng on 2019/7/22.
//

#include "MessageConductor.h"
#include "../lib/easylogging++.h"
#include "GameConfiguration.h"

using namespace z2;
using namespace std;
const function<void(istream &)> &z2::MessageConductor::getProcessor() const {
    return processor;
}

void z2::MessageConductor::setProcessor(const function<void(istream &)> &processor) {
    std::lock_guard<mutex> guard(processorMutex);
    MessageConductor::processor = processor;
}

bool z2::MessageConductor::isStopped() const {
    return stopped;
}

void z2::MessageConductor::sendMessage(stringstream &ss, int id) {
    ss << '\n';
    auto sock = connections[id];
    if (!sock) {
        LOG(WARNING) << "[Conductor] Connection not available: " << id;
    }
    string data = ss.str();
    sock->send(buffer(data,data.size()));
}


z2::MessageConductor::~MessageConductor() {
    stopped = true;
    service.stop();
}

z2::MessageConductor::MessageConductor(function<void(istream &)> processor) : processor(std::move(processor)) {

}


void z2::MessageConductor::start(int port, int count) {
    // creates the socket
    ip::tcp::endpoint ep(ip::tcp::v4(), port);
    ip::tcp::acceptor acc(service, ep);
//    server = std::make_shared<ip::tcp::socket>(service); // the
    connections = vector<socket_ptr>(count);
    buffers = vector<asio::streambuf>(count);
    for (int i = 0; i < count; i++) {
        auto sock = std::make_shared<ip::tcp::socket>(service);
        connections[i] = sock;
        acc.async_accept(*sock,[this,i](const error_code &error){
            handleAccept(error,i);
            return;
        });
    }

    // start the thread
    shared_ptr<MessageConductor> self(this);
    auto f = [this,self](){
        self->service.run();
        LOG(INFO) << "[Conductor] Ended!";
    };
    workingThread = std::thread(f);
    workingThread.detach();
}

void MessageConductor::handleAccept(const error_code &error, int id) {
    if(error){
        LOG(WARNING) << "[Conductor] Failed to connect!";
        return;
    }
    socket_ptr sock = connections[id];
    async_read_until(*sock, buffers[id], '\n',[this,id](const error_code &error, size_t length){
        handleReceive(error, length,id);
        return;
    });

}

void MessageConductor::handleReceive(const error_code &error, size_t length, int id) {
    if(error){
        LOG(WARNING) << "[Conductor] Failed to receive!";
        return;
    }
    LOG(INFO) << "[Conductor] Received from" << id;
    asio::streambuf &buf = buffers[id];
    std::istream input(&buf);
    {
        std::lock_guard<mutex> guard(processorMutex);
        processor(input);
    }
    buf.consume(1); // separator

    async_read_until(*connections[id], buffers[id], '\n',[this,id](const error_code &error, size_t length){
        handleReceive(error, length,id);
        return;
    });
}



