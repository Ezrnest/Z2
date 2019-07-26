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
using namespace std;
using namespace asio;
namespace z2 {
typedef ip::tcp::socket* socket_ptr;
class MessageConductor {

private:
    std::thread workingThread;
    bool stopped = false;
    function<void(istream&)> processor;

    shared_ptr<ip::tcp::acceptor> acceptor;

    vector<socket_ptr> connections;
    vector<asio::streambuf> buffers;
    mutex processorMutex;
//    socket_ptr server;
    io_service service;

    void handleAccept(const error_code &error, int id);

    void handleReceive(const error_code &error, size_t length, int id);
public:

    explicit MessageConductor(function<void(istream &)> processor);

    const function<void(istream &)> &getProcessor() const;

    void setProcessor(const function<void(istream &)> &processor);

    void start(int port, int count, const shared_ptr<MessageConductor> &self);


    bool isStopped() const;

    void sendMessage(stringstream &ss, int id);

//    void sendMessageAll(const string& str);

    virtual ~MessageConductor();
};

}
#endif //Z2_MESSAGECONDUCTOR_H
