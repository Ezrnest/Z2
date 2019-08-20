/*
 * Created by liyicheng on 2019/7/3.
 */

#ifndef Z2_SERVER_H
#define Z2_SERVER_H

#include <vector>
#include <memory>
#include "../world/World.h"

using namespace std;
namespace z2 {
class ClientProxy;

class Message;

class ControlMessage;
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
 * To start a game:
 * 1. A server is created.
 * 2. World of the server is set, either from a new world or a world loaded from save.
 * 3. Clients are created and registered to the server, and the world will be synchronized.
 * 4. after all the registers are finished, the server starts the game.
 */


/**
 *
 */
class Server {
public:
    enum class GameState {
        PAUSED, RUNNING
    };
private:
    /**
     * The clients that are registered to this server.
     */
    vector<shared_ptr<ClientProxy>> clients;
    /**
     * The world of this server.
     */
    shared_ptr<World> world;

    GameState gameState = GameState::PAUSED;

    /**
     * Broadcast the given message to all the clients.
     */
    void broadcastMessage(const shared_ptr<Message> &message);

    /**
     * Send the given message to one of the client.
     */
    void sendMessage(const shared_ptr<Message> &message, int clientId);

    /**
     * Calls the next player.
     */
    void callPlayer(int playerId);

    void shiftTurn();

    void onPlayerTurnFinish(const shared_ptr<Message> &message);

//    void onEndGame();

    bool checkGameReady();

    void dealWithControlMessage(const shared_ptr<z2::ControlMessage> &message);

    void attachListeners(const shared_ptr<World> &world);

public:

    virtual ~Server();

    /**
     * Accepts the given message.
     *
     * This method should be invoked logically by the player/(server port).
     */
    void acceptMessage(const shared_ptr<Message> &command);

    /**
     * Register the client to this server, the world will be synchronized.
     * The client will be assigned a client id.
     */
    bool registerClient(const shared_ptr<ClientProxy> &client);

    bool registerClient(const shared_ptr<ClientProxy> &client, int clientId);

    /**
     * Starts the game.
     */
    void startGame();

    void localEndGame(int winnerGroupId);

    void clientQuit(int clientId, const string &cause);

    void exceptionalEndGame(const string &cause);

    const shared_ptr<World> &getWorld() const;

    void setWorld(const shared_ptr<World> &world);

    const vector<shared_ptr<ClientProxy>> &getClients() const;

    GameState getGameState() const;


};

}

#endif //Z2_SERVER_H
