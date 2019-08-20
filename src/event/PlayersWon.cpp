/*
 * Created by liyicheng on 2019/8/20.
 */

#include "PlayersWon.h"

z2::PlayersWon::PlayersWon(const vector<int> &playerIds) : StateEvent(StateEventType::PlayersWon),
                                                           playerIds(playerIds) {}

const vector<int> &z2::PlayersWon::getPlayerIds() const {
    return playerIds;
}
