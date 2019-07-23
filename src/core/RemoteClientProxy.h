//
// Created by liyicheng on 2019/7/22.
//

#ifndef Z2_REMOTECLIENTPROXY_H
#define Z2_REMOTECLIENTPROXY_H

#include "ClientProxy.h"
#include "MessageConductor.h"

namespace z2 {
class Server;
class RemoteClientProxy : public ClientProxy{
private:
     shared_ptr<MessageConductor> conductor;
     int condId;
     weak_ptr<Server> server;
public:
    explicit RemoteClientProxy(shared_ptr<MessageConductor>  cond, int condId);

    void sendMessage(const shared_ptr<Message> &command) override;



    bool syncWorld(const shared_ptr<World> &world) override;
};
}

#endif //Z2_REMOTECLIENTPROXY_H
