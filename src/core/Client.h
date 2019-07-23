/*
 * Created by liyicheng on 2019/7/6.
 */

#ifndef Z2_CLIENT_H
#define Z2_CLIENT_H

#include <memory>
#include "Message.h"
#include "ClientProxy.h"
#include "ServerProxy.h"
#include "GameController.h"

/*
 * MODEL:
 * The model of the game at the client side: MVC
 *
 * Model: Holds the data
 * Controller: Respond to the events
 * View: The interface
 *
 * In this game,
 * World is the Model,
 * Client is the Controller,
 * GameGui is the view
 *
 */
using namespace std;
namespace z2 {
/**
 * Describes a client in the client side(remote).
 */
class Client : public GameController{
private:
public:
    /**
     * Accepts the given message.
     *
     * This method should be invoked logically by the server(port) part.
     */
    virtual void acceptMessage(const shared_ptr<Message> &command) = 0;

    /**
     * Sends the given message to the server.
     *
     * This method should be invoked logically by the player(/view) part.
     */
    void sendMessageToServer(const shared_ptr<Message>& message) override;

    /**
     * Sends a message of turn finish of the player of this client.
     */
    void sendTurnFinishMessage();

    /**
     * Sets the server port.
     */
    virtual void setServerProxy(const shared_ptr<ServerProxy> &server) = 0;

    virtual shared_ptr<ServerProxy> getServer() = 0;

    virtual ~Client();

    virtual shared_ptr<World> getWorld() = 0;

    virtual int getPlayerId() = 0;

    virtual const string& getPlayerName() = 0 ;


};
}

#endif //Z2_CLIENT_H
