/*
 * Created by liyicheng on 2019/7/7.
 */

#ifndef Z2_BOTCLIENTPORT_H
#define Z2_BOTCLIENTPORT_H

#include "core/ClientProxy.h"


namespace z2 {
class Server;


/**
 * The class of the bot.
 */
class BotClientPort : public ClientProxy{
protected:
    weak_ptr<Server> server;

    /**
     * Override this method to customize the bot behavior. To perform an operation,
     * this method should invoke `sendMessage` to send the message of the operation.
     * <br><b>Notice: Direct modification to the server's world will lead to world inconsistency between the server and client! </b>
     * <br>See class `GameMessage` for more info of messages.
     */
    void doBotTurn();

    void botTurnFinish();
public:

    const weak_ptr<Server> &getServer() const;

    void setServer(const shared_ptr<Server> &server);

    void sendMessage(const shared_ptr<Message> &command) override;

    bool syncWorld(const shared_ptr<World> &world) override;
};

}
#endif //Z2_BOTCLIENTPORT_H
