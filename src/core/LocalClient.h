/*
 * Created by liyicheng on 2019/7/6.
 */

#ifndef Z2_LOCALCLIENT_H
#define Z2_LOCALCLIENT_H

#include "Client.h"
#include "ClientProxy.h"
#include "Server.h"

namespace z2 {

class LocalClient : public Client, public ClientProxy {
private:
    weak_ptr<Server> server;
    weak_ptr<GameGui> gui;

    string playerName = "local";

    bool stopped = false;

    void dealWithControlMessage(const shared_ptr<ControlMessage> &message);

public:
    const weak_ptr<Server> &getRealServer() const;

    void setRealServer(const shared_ptr<Server> &server);

    /**
     * Ignores this method.
     */
    void setServerProxy(const shared_ptr<ServerProxy> &server) override;

    /**
     * Ignores this method.
     */
    shared_ptr<ServerProxy> getServer() override;

    void setView(const shared_ptr<GameGui> &gui) override;

    void acceptMessage(const shared_ptr<Message> &command) override;

    shared_ptr<World> getWorld() override;

    int getPlayerId() override;

    void sendMessage(const shared_ptr<Message> &command) override;

    bool syncWorld(const shared_ptr<World> &world) override;

    void sendMessageToServer(const MessagePtr &message) override;

    const string& getPlayerName() override ;

    void setPlayerName(const string &playerName);

    bool isGameRunning() override;
};

}
#endif //Z2_LOCALCLIENT_H
