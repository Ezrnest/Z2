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
#include <config/GameConfiguration.h>
#include "core/Server.h"
#include "bot/BotClientPort.h"
#include "LocalClient.h"

namespace z2 {
//Lobby::Lobby(const vector<PlayerType> &players, int port, shared_ptr<World> world)
//        : players(players), clients(players.size()), world(std::move(world)), port(port) {
//    openLobby(port);
//}
Lobby::Lobby(int port, const GameInitSetting &gameInitSetting) : port(port), gis(gameInitSetting),
                                                                 clients(gameInitSetting.getPlayerCount()) {
}

int getRemoteClientCount(const GameInitSetting &gis) {
    int c = 0;
    for (auto &it : gis.getPlayers()) {
        if (it.type == PlayerType::REMOTE_PLAYER) {
            c++;
        }
    }
    return c;
}

void Lobby::openLocalLobby() {
    // start local game instead
    onPlayerConnected(*this, -1);
    PLOG(plog::info) << "[Lobby] A local game is ready!";
    return;
}


void Lobby::openLobby() {
    int requiredCount = getRemoteClientCount(gis);
    if (requiredCount == 0) {
        openLocalLobby();
        return;
    }
    latch.reset(new CountDownLatch(requiredCount));
    auto listener = [this](const MessagePtr &msg, int cid) {
        shared_ptr<RegisterPlayer> rp = dynamic_pointer_cast<RegisterPlayer>(msg);
        if (!rp) {
            conductor->sendMessage(make_shared<SignalMessage>(SignalMessage::BAD), cid);// reject it
            return;
        }
        int id = rp->getPlayerId();
        if (id < 0 || id >= gis.getPlayers().size() ||
            gis.getPlayers()[id].type != PlayerType::REMOTE_PLAYER ||
            clients[id].first) {
            PLOG_WARNING << "[Lobby] Invalid remote player id = " << id;
            conductor->sendMessage(make_shared<SignalMessage>(SignalMessage::BAD), cid); // reject it
            return;
        }

        const string &name = rp->getPlayerName();
        auto *cp = new RemoteClientProxy(conductor, cid);
        PLOG_INFO << "[Lobby] RemoteClientProxy created, cid=" << cid;
        cp->sendMessage(make_shared<SignalMessage>(SignalMessage::GOOD));
        pair<shared_ptr<ClientProxy>, string> pair(shared_ptr<ClientProxy>(cp), name);
        clients[id] = pair;
        latch->countDown();
        onPlayerConnected(*this, id);
        PLOG(plog::info) << "[Lobby] Player: " << name << " connected!";
        return;
    };
    conductor = std::make_shared<MessageConductor>(listener);
    conductor->start(port, requiredCount, conductor);
    PLOG(plog::info) << "[Lobby] Lobby opened!";
}


void Lobby::initNames(const shared_ptr<World> &world) {
    auto &players = gis.getPlayers();
    for (int i = 0; i < players.size(); i++) {
        switch (players[i].type) {
            case PlayerType::LOCAL_PLAYER: {
                world->getPlayer(i).setName(GameConfiguration::instance().getPlayerName());
                break;
            }
            case PlayerType::BOT_PLAYER: {
                break;
            }
            case PlayerType::REMOTE_PLAYER: {
                auto &p = clients[i];
                world->getPlayer(i).setName(p.second);
                break;
            }
        }

    }
}

void setGameGui(const weak_ptr<GameGui> &gui, shared_ptr<LocalClient> &lc) {
    auto view = gui.lock();
    view->setControllerAndView(lc);
    lc->setView(view);
}

void Lobby::setupRemoteClients(shared_ptr<Server> &server, const vector<PlayerSetting> &players) const {
    for (int i = 0; i < players.size(); i++) {
        if (players[i].type == PlayerType::REMOTE_PLAYER) {
            auto &p = clients[i];
            server->registerClient(p.first, players[i].playerId);
        }
    }
}

shared_ptr<Server> Lobby::startLocalGame(const weak_ptr<GameGui> &gui) {
    auto pair = gis.buildLocalGame();
    auto server = pair.first;
    auto localClient = pair.second;
    setGameGui(gui, localClient);
    server->startGame();
    return server;
}


shared_ptr<Server> Lobby::startGame(const weak_ptr<GameGui> &gui, int timeOut) {
    if (getRemoteClientCount(gis) == 0) {
        return startLocalGame(gui);
    }
    bool waitRe = latch->await(std::chrono::milliseconds(timeOut));
    if (!waitRe) {
        PLOG_WARNING << "[Lobby] Wait timeout!";
        return shared_ptr<Server>();
    }
    auto pair = gis.buildLocalGame();
    auto server = pair.first;
    initNames(server->getWorld());
    auto localClient = pair.second;
    setGameGui(gui, localClient);
    auto &players = gis.getPlayers();
    setupRemoteClients(server, players);

    weak_ptr<Server> ws = server;
    auto listener = [ws](const MessagePtr &msg, int) {
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
    FailureProcessor fp = [ws](asio::error_code ec, int cid) {
        if (!ws.expired()) {
            auto s = ws.lock();
            for (auto &c : s->getClients()) {
                auto rc = dynamic_pointer_cast<RemoteClientProxy>(c);
                if (rc && cid == rc->getCondId()) {
                    s->clientQuit(rc->getClientId(), "");
                    return;
                }
            }
            s->exceptionalEndGame(ec.message());
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
        PLOG_INFO << "[Lobby] All reset";
    }
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
    if (conductor) {
        return conductor->getLocalAddressInfo();
    }
    return "127.0.0.1";
//    auto s = ip::address_v4::from_string("localhost");
//    stringstream ss;
//    ss << s.to_string() << ':' << port;
//    return ss.str();
}

bool Lobby::isGameReady() {
    return getRemoteClientCount(gis) == 0 || latch->getCount() == 0;
}

Lobby::~Lobby() {
    closeLobby();
}

string Lobby::getHostNameInfo() {
    if (conductor) {
        return conductor->getLocalHostNameInfo();
    }
    return "Unknown";
}


}