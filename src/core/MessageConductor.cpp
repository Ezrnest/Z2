#include <memory>

#include <utility>
#include <sstream>

//
// Created by liyicheng on 2019/7/22.
//

#include "MessageConductor.h"
#include "plog/Log.h"
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
    if(stopped){
        PLOG_WARNING << "[Conductor] Conductor is stopped!";
        return;
    }

    ss << '\n';
    auto sock = connections[id];
    if (!sock) {
        PLOG_WARNING << "[Conductor] Connection not available: " << id;
        return;
    }
    string data = ss.str();
    sock->send(buffer(data, data.size()));
}


z2::MessageConductor::~MessageConductor() {
    stopped = true;
    service.stop();
}

z2::MessageConductor::MessageConductor(function<void(istream &)> processor) : processor(std::move(processor)) {
}


void z2::MessageConductor::start(int port, int count, const shared_ptr<MessageConductor> &self) {
    // creates the socket
    ip::tcp::endpoint ep(ip::tcp::v4(), port);
    acceptor.reset(new ip::tcp::acceptor(service, ep));
//    server = std::make_shared<ip::tcp::socket>(service); // the
    connections = vector<socket_ptr>(count);
    buffers = vector<asio::streambuf>(count);
    for (int i = 0; i < count; i++) {
        auto sock = new ip::tcp::socket(service);
        connections[i] = sock;
        acceptor->async_accept(*sock, [this, i](const error_code &error) {
            handleAccept(error, i);
            return;
        });
    }

    // start the thread
    auto f = [this, self]() {
        service.run();
        PLOG(plog::info) << "[Conductor] Ended!";
        stopped = true;
    };
    workingThread = std::thread(f);
    workingThread.detach();
}

void MessageConductor::handleAccept(const error_code &error, int id) {
    if (error) {
        PLOG_WARNING << "[Conductor] Failed to connect, error code = " << error.value() << ", message = "
                     << error.message();
        return;
    }
    PLOG_INFO << "[Conductor] Accepted id = " << id;
    socket_ptr sock = connections[id];
    async_read_until(*sock, buffers[id], '\n', [this, id](const error_code &error, size_t length) {
        handleReceive(error, length, id);
        return;
    });
}

//#include <iostream>
void MessageConductor::handleReceive(const error_code &error, size_t length, int id) {
    if (error) {
        PLOG_WARNING << "[Conductor] Failed to receive, error = " << error.value();
        return;
    }
    PLOG(plog::info) << "[Conductor] Received from " << id;

    asio::streambuf &buf = buffers[id];
//    std::cout << &buf;

    std::istream input(&buf);
    {
        std::lock_guard<mutex> guard(processorMutex);
        processor(input);
    }
    string s;
    std::getline(input,s);

    async_read_until(*connections[id], buffers[id], '\n', [this, id](const error_code &error, size_t length) {
        handleReceive(error, length, id);
        return;
    });
}



