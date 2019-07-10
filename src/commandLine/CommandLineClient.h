/*
 * Created by liyicheng on 2019/7/4.
 */

#ifndef Z2_COMMANDLINECLIENT_H
#define Z2_COMMANDLINECLIENT_H

#include "../core/ClientProxy.h"
#include "../core/Server.h"
#include "../core/Client.h"

namespace z2 {
class CommandLineClient : public Client, public ClientProxy{
private:
    shared_ptr<Server> server;
public:

    const shared_ptr<Server> &getServer() const;

    void setServer(const shared_ptr<Server> &server);

    void sendMessage(const shared_ptr<Message> &command) override;

    bool syncWorld(const World &world) override;


};
}

#endif //Z2_COMMANDLINECLIENT_H
