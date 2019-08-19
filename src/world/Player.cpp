/*
 * Created by liyicheng on 2019/7/4.
 */

#include <core/Serializable.h>
#include "Player.h"

using namespace z2;

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
    output << name << ' '
           << playerId_ << ' '
           << groupId_ << ' '
           << colorCode << ' '
           << gold_ << ' '
           << dead << ' '
           << techPoints << ' '
           << maxTechPoints << ' ';
    Serializable::serializeCollection(technologies, output);
}

void z2::Player::loadDataFrom(istream &input) {
    input >> name >> playerId_ >> groupId_ >> colorCode >> gold_ >> dead >> techPoints >> maxTechPoints;
    Serializable::deserializeCollection<string, set<string>>(technologies, input);
}

bool z2::Player::requireGold(int amount) {
    if (amount > gold_) {
        return false;
    } else {
        gold_ -= amount;
        return true;
    }
}

void z2::Player::addGold(int delta) {
    gold_ += delta;
}

bool z2::Player::isDead() const {
    return dead;
}

void z2::Player::setDead(bool dead) {
    Player::dead = dead;
}

const string &z2::Player::getName() const {
    return name;
}

void z2::Player::setName(const string &name) {
    Player::name = name;
}


void z2::Player::addTech(const string &tech) {
    technologies.insert(tech);
}

int Player::getTechPoints() const {
    return techPoints;
}

void Player::setTechPoints(int techPoints) {
    Player::techPoints = techPoints;
}

int Player::getMaxTechPoints() const {
    return maxTechPoints;
}

void Player::setMaxTechPoints(int maxTechPoints) {
    Player::maxTechPoints = maxTechPoints;
}

void Player::refreshTechPoints() {
    techPoints = maxTechPoints;
}

void Player::consumeTechPoint() {
    if (techPoints > 0) {
        techPoints--;
    }
}

const set<string> &Player::getTechnologies() const {
    return technologies;
}

int Player::getColorCode() const {
    return colorCode;
}

void Player::setColorCode(int colorCode) {
    Player::colorCode = colorCode;
}

bool Player::isAlly(int group) const{
    if(groupId_ == NO_GROUP || group == NO_GROUP){
        return false;
    }
    return groupId_ == group;
}


