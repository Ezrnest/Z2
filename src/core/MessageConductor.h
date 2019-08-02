//
// Created by liyicheng on 2019/7/22.
//

#ifndef Z2_MESSAGECONDUCTOR_H
#define Z2_MESSAGECONDUCTOR_H


#define ASIO_STANDALONE

#include "asio.hpp"
#include <unordered_map>
#include <vector>
#include <mutex>
#include "Message.h"
#include <thread>
using namespace std;
using namespace asio;

namespace z2 {
typedef ip::tcp::socket *socket_ptr;

using MessageProcessor = function<void(const MessagePtr &)>;
using FailureProcessor = function<void(asio::error_code, int)>;

class MessageConductor {

private:
    std::thread workingThread;
    bool stopped = false;


    MessageProcessor processor;
    FailureProcessor failureProcessor = [](asio::error_code, int){};

    shared_ptr<ip::tcp::acceptor> acceptor;

    vector<socket_ptr> connections;
    vector<asio::streambuf> buffers;
    mutex processorMutex;
//    socket_ptr server;
    shared_ptr<io_service> service;

    void handleAccept(const error_code &error, int id);

    void handleReceive(const error_code &error, size_t length, int id);

public:

    explicit MessageConductor(MessageProcessor processor);

    const MessageProcessor &getProcessor() const;

    void setProcessor(const MessageProcessor &processor);

    void setFailureProcessor(const FailureProcessor &failureProcessor);

    void start(int port, int count, const shared_ptr<MessageConductor> &self);

    void stop();

    bool isStopped() const;

    void sendMessage(const MessagePtr &msg, int id);

//    void sendMessageAll(const string& str);

    virtual ~MessageConductor();

    static asio::error_code sendMessageToSocket(socket_ptr socket, const MessagePtr &message);

    static MessagePtr readMessageFromSocket(asio::streambuf &buffer);
};

}
#endif //Z2_MESSAGECONDUCTOR_H
