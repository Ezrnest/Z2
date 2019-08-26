/*
 * Created by liyicheng on 2019/7/7.
 */

#ifndef Z2_BOTCLIENTPROXY_H
#define Z2_BOTCLIENTPROXY_H

#include "core/ClientProxy.h"
#include "Bot.h"


namespace z2 {
class Server;


/**
 * The class of the bot.
 */
class BotClientProxy : public ClientProxy, public enable_shared_from_this<BotClientProxy>{
private:
    shared_ptr<Bot> bot;

    void performTurn();

    void botTurnFinish();
protected:
    weak_ptr<Server> server;


public:

    explicit BotClientProxy(shared_ptr<Bot> bot);

    BotClientProxy();

    const weak_ptr<Server> &getServer() const;

    void setServer(const shared_ptr<Server> &server);

    void sendMessage(const shared_ptr<Message> &command) override;

    bool syncWorld(const shared_ptr<World> &world) override;
};

}
#endif //Z2_BOTCLIENTPROXY_H
