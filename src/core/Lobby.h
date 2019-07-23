//
// Created by liyicheng on 2019/7/22.
//

#ifndef Z2_LOBBY_H
#define Z2_LOBBY_H
#include <memory>
#include <string>
#include <vector>
#include <mutex>
#include "GameGui.h"
#include "MessageConductor.h"
#include "util/CountDownLatch.h"

using namespace std;
namespace z2 {
class Server;


enum class PlayerType {
    LOCAL_PLAYER,
    BOT_PLAYER,
    REMOTE_PLAYER
};

class Lobby {
private:
    vector<PlayerType> players;
    shared_ptr<World> world;
    vector<pair<shared_ptr<ClientProxy>,string>> clients;

    shared_ptr<MessageConductor> conductor;

    bool closed = false;

    int connectionCount = 0;


    shared_ptr<CountDownLatch> latch;

    function<void(Lobby&,int)> onPlayerConnected = [](Lobby&,int){};

    void openLobby(int port);

public:
    /**
     * Creates and opens a lobby.
     */
    explicit Lobby(const vector<PlayerType> &players, int port, shared_ptr<World>  world);

    /**
     * A blocking method, waiting all the players to join in.
     */
    shared_ptr<Server> startGame(const weak_ptr<GameGui> &gui, int timeOut);

    const vector<PlayerType> &getPlayers() const;

    const vector<pair<shared_ptr<ClientProxy>, string>> &getClients() const;

    const function<void(Lobby &, int)> &getOnPlayerConnected() const;

    void setOnPlayerConnected(const function<void(Lobby &, int)> &onPlayerConnected);

    void closeLobby();

};
}

#endif //Z2_LOBBY_H
