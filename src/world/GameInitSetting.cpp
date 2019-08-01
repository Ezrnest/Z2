#include <utility>

#include <utility>

#include <utility>

//
// Created by liyicheng on 2019/7/29.
//

#include "GameInitSetting.h"
#include <vector>
#include <core/LocalClient.h>
#include "GameMap.h"
#include "core/Server.h"
#include "bot/BotClientPort.h"
using namespace std;
namespace z2 {


PlayerSetting::PlayerSetting(int playerId, int positionId, int groupId, PlayerType type) : playerId(playerId),
                                                                                           positionId(positionId),
                                                                                           groupId(groupId),
                                                                                           type(type) {}

PlayerSetting::PlayerSetting() : PlayerSetting(-1, -1, -1, PlayerType::BOT_PLAYER) {

}

GameInitSetting::GameInitSetting(int playerCount, shared_ptr<GameMap> map) : players(playerCount), map(std::move(map)) {

}

void GameInitSetting::setupPlayer(int idx, const PlayerSetting &s) {
    players[idx] = s;
}

bool GameInitSetting::isValidLocalSetting(bool isLocalGame) {
    vector<bool> usedPositions(map->getMaxPlayerCount());
    for (auto &&usedPosition : usedPositions) {
        usedPosition = false;
    }
    bool hasPlayer = false;
    for (int i = 0; i < players.size(); i++) {
        auto &ps = players[i];
        if (isLocalGame && ps.type == PlayerType::REMOTE_PLAYER) {
            return false;
        }
        if (ps.type == PlayerType::LOCAL_PLAYER) {
            if (hasPlayer) {
                return false;
            }
            hasPlayer = true;
        }
        if (ps.playerId != i) {
            return false;
        }
        int pos = ps.positionId;
        if (pos < 0 || pos >= usedPositions.size()) {
            return false;
        }
        if (usedPositions[pos]) {
            return false;
        }
        usedPositions[pos] = true;
    }
    return hasPlayer;
}

GameInitSetting::GameInitSetting(vector<PlayerSetting> players, shared_ptr<GameMap> map) : players(std::move(
        players)), map(std::move(map)) {}

shared_ptr<World> GameInitSetting::buildWorld() {
    return map->buildWorld(players);
}

pair<shared_ptr<Server>, shared_ptr<LocalClient>> GameInitSetting::buildLocalGame() {
    auto w = buildWorld();
    shared_ptr<Server> server(new Server());
    server->setWorld(w);
    shared_ptr<LocalClient> lc;
    for (auto & ps : players) {
        switch (ps.type) {
            case PlayerType::LOCAL_PLAYER: {
                lc.reset(new LocalClient());
                lc->setRealServer(server);
                server->registerClient(lc);
                break;
            }
            case PlayerType::BOT_PLAYER: {
                shared_ptr<BotClientPort> bc(new BotClientPort());
                bc->setServer(server);
                server->registerClient(bc);
                break;
            }
            case PlayerType::REMOTE_PLAYER: {
                break;
            }
        }
    }
    return make_pair(server, lc);

}
}