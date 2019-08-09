#include <memory>

#include <memory>

#include <utility>

#include <memory>

//
// Created by liyicheng on 2019/7/22.
//

#include "Lobby.h"
#include "messages/RegisterPlayer.h"
#include "RemoteClientProxy.h"
#include <thread>
#include "plog/Log.h"
#include <core/messages/SignalMessage.h>
#include "core/Server.h"
#include "bot/BotClientPort.h"
#include "LocalClient.h"

namespace z2 {


int getRemoteClientCount(const vector<PlayerType> &v) {
    int c = 0;
    for (auto &it : v) {
        if (it == PlayerType::REMOTE_PLAYER) {
            c++;
        }
    }
    return c;
}

void Lobby::openLobby(int port) {
    int requiredCount = getRemoteClientCount(players);
    latch.reset(new CountDownLatch(requiredCount));
    function<void(const MessagePtr &)> listener = [this](const MessagePtr &msg) {
        shared_ptr<RegisterPlayer> rp = dynamic_pointer_cast<RegisterPlayer>(msg);
        if (!rp) {
            return;
        }
        int id = rp->getPlayerId();
        if (id < 0 || id >= players.size() || players[id] != PlayerType::REMOTE_PLAYER) {
            PLOG_WARNING << "[Lobby] Invalid remote player id = " << id;
            return;
        }

        const string &name = rp->getPlayerName();
        auto *cp = new RemoteClientProxy(conductor, connectionCount++);
        cp->sendMessage(make_shared<SignalMessage>(SignalMessage::GOOD));
        pair<shared_ptr<ClientProxy>, string> pair(shared_ptr<ClientProxy>(cp), name);
        clients[id] = pair;
        latch->countDown();
        onPlayerConnected(*this, id);
        PLOG(plog::info) << "[Lobby] Player connected!";
        return;
    };
    conductor = std::make_shared<MessageConductor>(listener);
    conductor->start(port, requiredCount, conductor);
    PLOG(plog::info) << "[Lobby] Lobby opened!";
}

Lobby::Lobby(const vector<PlayerType> &players, int port, shared_ptr<World> world)
        : players(players), clients(players.size()), world(std::move(world)), port(port) {
    openLobby(port);
}


shared_ptr<Server> Lobby::startGame(const weak_ptr<GameGui> &gui, int timeOut) {
    bool waitRe = latch->await(std::chrono::milliseconds(timeOut));
    if (!waitRe) {
        PLOG_WARNING << "[Lobby] Wait timeout!";
        return shared_ptr<Server>();
    }
    auto server = make_shared<Server>();
    server->setWorld(world);
    int botCount = 0;
    for (int i = 0; i < players.size(); i++) {
        switch (players[i]) {
            case PlayerType::LOCAL_PLAYER: {
                auto cl = std::make_shared<LocalClient>();
                cl->setRealServer(server);
                server->registerClient(cl);
                auto view = gui.lock();
                cl->setView(view);
                view->setControllerAndView(cl);
                world->getPlayer(i).setName("LocalPlayer");
                break;
            }
            case PlayerType::BOT_PLAYER: {
                auto cl = std::make_shared<BotClientPort>();
                cl->setServer(server);
                server->registerClient(cl);

                stringstream ss;
                botCount++;
                ss << "Bot" << botCount;
                world->getPlayer(i).setName(ss.str());

                break;
            }
            case PlayerType::REMOTE_PLAYER: {
                auto &p = clients[i];
                world->getPlayer(i).setName(p.second);
                server->registerClient(p.first);
                break;
            }
        }

    }
    weak_ptr<Server> ws = server;
    function<void(const MessagePtr &)> listener = [ws](const MessagePtr &msg) {
        if (!msg) {
            PLOG_WARNING << "[Lobby] Failed to accept message!";
            return;
        }
        PLOG(plog::info) << "[Lobby] Received message: " << msg->getClassName();
        if (!ws.expired()) {
            ws.lock()->acceptMessage(msg);
        }
    };
    conductor->setProcessor(listener);
    FailureProcessor fp = [ws](asio::error_code ec, int) {
        if (!ws.expired()) {
            ws.lock()->exceptionalEndGame(ec.message());
        }
    };
    conductor->setFailureProcessor(fp);
    server->startGame();
    return server;
}

void Lobby::closeLobby() {
    if (conductor) {
        conductor->stop();
        conductor.reset();
    }
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

string Lobby::getAddressInfo() {
    auto s = ip::address_v4::loopback();
    stringstream ss;
    ss << s.to_string() << ':' << port;
    return ss.str();
}

bool Lobby::isGameReady() {
    return latch->getCount() == 0;
}

Lobby::~Lobby() {
    closeLobby();
}

}