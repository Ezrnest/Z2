//
// Created by liyicheng on 2019/7/23.
//

#ifndef Z2_REMOTECLIENT_H
#define Z2_REMOTECLIENT_H

#include <core/messages/ControlMessage.h>
#include <core/messages/SyncWorld.h>
#include "Client.h"

namespace z2 {
class RemoteClient : public Client{
private:
    shared_ptr<World> world;

    void dealWithControlMessage(const shared_ptr<ControlMessage>& message);

//    void dealWithControlMessage(const shared_ptr<ControlMessage>& message);
    weak_ptr<GameGui> gui;

    shared_ptr<ServerProxy> serverProxy;

    int playerId;

    string playerName = "remote";

    void dealWithSyncWorld(const shared_ptr<SyncWorld> &msg);

public:
    explicit RemoteClient(int playerId);


    void acceptMessage(const shared_ptr<Message> &command) override;

    void setServerProxy(const shared_ptr<ServerProxy> &server) override;

    shared_ptr<ServerProxy> getServer() override;

    shared_ptr<World> getWorld() override;

    int getPlayerId() override;

    void setPlayerId(int playerId);

    const string &getPlayerName() override ;

    void setPlayerName(const string &playerName);

    void sendMessageToServer(const MessagePtr &message) override;

    void setView(const shared_ptr<GameGui> &gui) override;

    void onConnectionLost() override;

    bool isGameRunning() override;
};
}

#endif //Z2_REMOTECLIENT_H
