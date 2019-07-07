/*
 * Created by liyicheng on 2019/7/6.
 */

#ifndef Z2_LOCALCLIENT_H
#define Z2_LOCALCLIENT_H

#include "Client.h"
#include "ClientPort.h"
#include "Server.h"

namespace z2 {

class LocalClient : public Client, public virtual ClientPort {
private:
    shared_ptr<Server> server;
    shared_ptr<GameGui> gui;
    int playerId;
public:
    const shared_ptr<Server> &getRealServer() const;

    void setRealServer(const shared_ptr<Server> &server);

    /**
     * Ignores this method.
     */
    void setServerPort(const shared_ptr<ServerPort> &server) override;

    /**
     * Ignores this method.
     */
    shared_ptr<ServerPort> getServer() override;

    void setView(const shared_ptr<GameGui> &gui) override;

    void acceptMessage(const shared_ptr<Message> &command) override;

    shared_ptr<World> getWorld() override;

    int getPlayerId() override;

    void sendMessage(const shared_ptr<Message> &command) override;

    bool syncWorld(const World &world) override;

    void sendMessageToServer(const MessagePtr &message) override;
};

}
#endif //Z2_LOCALCLIENT_H
