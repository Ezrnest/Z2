#include <utility>

#include <memory>

//
// Created by liyicheng on 2019/7/22.
//

#include "Lobby.h"
#include "messages/RegisterPlayer.h"
#include "RemoteClientProxy.h"
#include <thread>
#include <easylogging++.h>
#include "core/Server.h"
namespace z2 {


int getRemoteClientCount(const vector<PlayerType>& v){
    int c = 0;
    for (auto &it : v) {
        if (it == PlayerType::REMOTE_PLAYER) {
            c ++;
        }
    }
    return c;
}

void Lobby::openLobby(int port) {
    int requiredCount = getRemoteClientCount(players);
    latch.reset(new CountDownLatch(requiredCount));
    auto listener =[this](istream& input){
        shared_ptr<Message> msg(dynamic_cast<Message*>(SerializableRegistry::instance().deserialize(input)));
        shared_ptr<RegisterPlayer> rp = dynamic_pointer_cast<RegisterPlayer>(msg);
        if(!rp){
            return;
        }
        int id = rp->getPlayerId();
        if(players[id] != PlayerType::REMOTE_PLAYER){
            return;
        }

        const string& name = rp->getPlayerName();
        RemoteClientProxy *cp = new RemoteClientProxy(conductor,connectionCount++);
        pair<shared_ptr<ClientProxy>,string> pair(shared_ptr<ClientProxy>(cp),name);
        clients[id] = pair;
        latch->countDown();
        onPlayerConnected(*this, id);
        LOG(INFO)  << "Player connected!";
        return;
    };
    conductor = std::make_shared<MessageConductor>(listener);
    conductor->start(port,requiredCount);
}

Lobby::Lobby(const vector<PlayerType> &players, int port, shared_ptr<World>  world)
        : players(players), clients(players.size()), world(std::move(world)){
    openLobby(port);
}


shared_ptr<Server> Lobby::startGame(const weak_ptr<GameGui> &gui, int timeOut) {
    latch->await(std::chrono::milliseconds(timeOut));
    auto server = make_shared<Server>();
    server->setWorld(world);
    for(int i=0;i<players.size();i++){
        auto &p = clients[i];
        world->getPlayer(i).setName(p.second);
        server->registerClient(p.first);
    }
    weak_ptr<Server> ws = server;
    auto listener =[ws](istream& input){
        shared_ptr<Message> msg(dynamic_cast<Message*>(SerializableRegistry::instance().deserialize(input)));
        if(!msg){
            LOG(WARNING) << "Failed to accept message!";
            return;
        }
        LOG(INFO)  << "Received message: " << msg->getClassName();
        ws.lock()->acceptMessage(msg);
        return;
    };
    conductor->setProcessor(listener);
    server->startGame();
    return server;
}

void Lobby::closeLobby() {

}

const vector<PlayerType> &Lobby::getPlayers() const {
    return players;
}

const vector<pair<shared_ptr<ClientProxy>, string>> &Lobby::getClients() const {
    return clients;
}

const function<void(Lobby &, int)> &Lobby::getOnPlayerConnected() const {
    return onPlayerConnected;
}

void Lobby::setOnPlayerConnected(const function<void(Lobby &, int)> &onPlayerConnected) {
    Lobby::onPlayerConnected = onPlayerConnected;
}

}