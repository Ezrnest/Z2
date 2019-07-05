/*
 * Created by liyicheng on 2019/7/3.
 */

#ifndef Z2_CLIENT_H
#define Z2_CLIENT_H

#include "Message.h"
#include "../world/World.h"
#include <memory>

namespace z2 {

/**
 * Client represents a logic part in the game.
 * It is a message receiver that can receive the message of updates in the game.
 * It represents a player who can get commands and do actions. The client id matches the corresponding player id.
 *
 * There can be different implementations for this interface.
 */
class Client {
private:
    int clientId = -1;
public:


    virtual ~Client();

    int getClientId() const;

    void setClientId(int clientId_);

    /**
     * Sends a message to the client.
     * Performs the given command in the client world.
     */
    virtual void sendMessage(const shared_ptr<Message> &command) = 0;

//    /**
//     * Notifies the player of the given id to start the turn.
//     */
//    virtual void onTurn(int playerId) = 0;

    /**
     * Synchronizes the client world with the given server world.
     * @return `true` if the operation succeed, otherwise false.
     */
    virtual bool syncWorld(const World &world) = 0;
};

}

#endif //Z2_CLIENT_H
