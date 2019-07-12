/*
 * Created by liyicheng on 2019/7/4.
 */

#include "Player.h"

int z2::Player::getPlayerId() const {
    return playerId_;
}

void z2::Player::setPlayerId(int playerId) {
    Player::playerId_ = playerId;
}

int z2::Player::getGroupId() const {
    return groupId_;
}

void z2::Player::setGroupId(int groupId) {
    Player::groupId_ = groupId;
}

int z2::Player::getGold() const {
    return gold_;
}

void z2::Player::setGold(int gold) {
    Player::gold_ = gold;
}

void z2::Player::saveDataTo(ostream &output) {
    output << playerId_ << ' '
           << groupId_ << ' '
           << gold_ << ' ';
}

void z2::Player::loadDataFrom(istream &input) {
    input >> playerId_ >> groupId_ >> gold_;
}

bool z2::Player::requireGold(int amount) {
    if(amount > gold_){
        return false;
    }else{
        gold_ -= amount;
        return true;
    }
}

void z2::Player::addGold(int delta) {
    gold_ += delta;
}
