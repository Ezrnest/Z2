//
// Created by liyicheng on 2019/7/22.
//

#ifndef Z2_LOBBY_H
#define Z2_LOBBY_H
#include "util/CountDownLatch.h"
#include <memory>
#include <string>
#include <vector>
#include <mutex>
#include "GameGui.h"
#include "MessageConductor.h"
#include "world/GameInitSetting.h"
using namespace std;
namespace z2 {
class Server;




class Lobby {
private:

    GameInitSetting gis;

    vector<pair<shared_ptr<ClientProxy>,string>> clients;

    shared_ptr<MessageConductor> conductor;

    bool closed = false;

//    int connectionCount = 0;

    int port;

    shared_ptr<CountDownLatch> latch;

    function<void(Lobby&,int)> onPlayerConnected = [](Lobby&,int){return;};



    void initNames(const shared_ptr<World>& world);

    void openLocalLobby();

    shared_ptr<Server> startLocalGame(const weak_ptr<GameGui> &gui);

public:
    /**
     * Creates and opens a lobby.
     */
    explicit Lobby(int port, const GameInitSetting& gameInitSetting);

    /**
     * Opens the lobby, enabling connection.
     */
    void openLobby();

    virtual ~Lobby();

    string getAddressInfo();

    string getHostNameInfo();

    /**
     * A blocking method, waiting all the players to join in.
     */
    shared_ptr<Server> startGame(const weak_ptr<GameGui> &gui, int timeOut);

    bool isGameReady();

    const vector<pair<shared_ptr<ClientProxy>, string>> &getClients() const;

    const function<void(Lobby &, int)> &getOnPlayerConnected() const;

    void setOnPlayerConnected(const function<void(Lobby &, int)> &onPlayerConnected);

    void closeLobby();

    void setupRemoteClients(shared_ptr<Server> &server, const vector<PlayerSetting> &players) const;
};
}

#endif //Z2_LOBBY_H
