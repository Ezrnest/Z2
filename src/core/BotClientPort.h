/*
 * Created by liyicheng on 2019/7/7.
 */

#ifndef Z2_BOTCLIENTPORT_H
#define Z2_BOTCLIENTPORT_H

#include "ClientProxy.h"


namespace z2 {
class Server;


class BotClientPort : public ClientProxy{
private:
    weak_ptr<Server> server;

    void doBotTurn();

    void botTurnFinish();
public:

    const weak_ptr<Server> &getServer() const;

    void setServer(const shared_ptr<Server> &server);

    void sendMessage(const shared_ptr<Message> &command) override;

    bool syncWorld(const World &world) override;
};

}
#endif //Z2_BOTCLIENTPORT_H
