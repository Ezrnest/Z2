/*
 * Created by liyicheng on 2019/7/3.
 */

#ifndef Z2_SERVER_H
#define Z2_SERVER_H

#include <vector>
#include <memory>
#include "ClientPort.h"
#include "../world/World.h"

using namespace std;
namespace z2 {

/*
 * MODEL:
 * The model of the game: C/S model.
 *
 * ---------------------------------------------------
 *
 * Local Game Structure:
 *
 * Server <-> LocalClientPort=LocalServerPort=Client
 *
 * ---------------------------------------------------
 *
 * Remote Game Structure:
 * Local:                            |  Remote:
 * Server <-> LocalClient (bots)     |
 *        <-> LocalClient            |
 *            (Player1, local)       |
 *        <-> RemoteClientPort  >> (TCP) << RemoteServerPort <-> Client [At computer 2]
 *            (Player2, remote)      |
 *        <-> RemoteClientPort  >> (TCP) << RemoteServerPort <-> Client [At computer 3]
 *            (Player3, remote)      |
 *        ...                        |
 * ---------------------------------------------------
 *
 * PROCESS:
 * To start a game, firstly, a server is created.
 * Second, clients are created and registered to the server.
 * Third, after all the registers are finished, the server starts the game.
 */


/**
 *
 */
class Server {
private:
    /**
     * The clients that are registered to this server.
     */
    vector<shared_ptr<ClientPort>> clients;
    /**
     * The world of this server.
     */
    shared_ptr<World> world;
    enum GameState{
        PAUSED,RUNNING
    };
    int gameState = PAUSED;
    /**
     * Broadcast the given message to all the clients.
     */
    void broadcastMessage(const shared_ptr<Message>& message);

    /**
     * Send the given message to one of the client.
     */
    void sendMessage(const shared_ptr<Message>& message, int clientId);

    /**
     * Calls the next player.
     */
    void callPlayer(int playerId);

    void shiftTurn();

    void onPlayerTurnFinish(const shared_ptr<Message>& message);
public:

    virtual ~Server();
    /**
     * Accepts the given message.
     *
     * This method should be invoked logically by the player/(server port).
     */
    void acceptMessage(const shared_ptr<Message> &command);

    /**
     * Register the client to this server.
     * The client will be assigned a client id and the world will be synchronized.
     */
    bool registerClient(const shared_ptr<ClientPort>& client);

    /**
     * Starts the game, or restart the game.
     */
    void startGame();

    /**
     * Pause the game, when pausing, all the message send will be discarded.
     */
    void pauseGame();

    const shared_ptr<World> &getWorld() const;

    void setWorld(const shared_ptr<World> &world);

    int getGameState() const;
};

}

#endif //Z2_SERVER_H
