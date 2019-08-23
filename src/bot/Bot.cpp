//
// Created by liyicheng on 2019/8/10.
//

#include "Bot.h"
#include <chrono>
#include <memory>
#include <thread>
#include <core/messages/UnitMove.h>


namespace z2 {


void Bot::init(const shared_ptr<Server> &s, const shared_ptr<World> &w, int pid) {
    server = s.get();
    world = w.get();
    playerId = pid;
    randomEngine(); // make it random
}

void Bot::doBotTurn() {

}

void Bot::doInit() {

}

void Bot::makeOperation(const shared_ptr<GameMessage> &msg) {
    if (server) {
        server->acceptMessage(msg);
    }
}

int Bot::randomIntBetween(int downerInclusive, int upperExclusive) {
    std::uniform_int_distribution<int> dis(downerInclusive, upperExclusive - 1);
    return dis(randomEngine);
}

void Bot::sleepFor(long millisecond) {
    std::this_thread::sleep_for(std::chrono::milliseconds(millisecond));
}

Bot::Bot() {
    randomEngine();

}

BotDifficulty Bot::getDifficulty() {
    return difficulty;
}

Bot::Bot(BotDifficulty difficulty) : difficulty(difficulty) {}

bool Bot::moveToward(unsigned int unitId, const Point &dest) {
    if (!world->isInside(dest)) {
        return false;
    }
    auto en = world->getEntity(unitId);
    if (en->getOwnerId() != playerId) {
        return false;
    }
    auto unit = dynamic_pointer_cast<GameUnit>(en);
    if (!unit) {
        return false;
    }
    auto &from = unit->getPos();
    if (from == dest) {
        return true;
    }
    int moves = unit->getRemainingMoves();
    if (moves == 0) {
        return false;
    }
    auto path = world->findPath(from, dest, unit);
    if (path.empty()) {
        return false;
    }
    for (size_t i = path.size() - 1; i >= 0; i--) {
        auto t = path[i];
        if (t.second <= moves) {
            makeOperation(std::make_shared<UnitMove>(from, t.first));
            if (t.first == dest) {
                return true;
            }
            break;
        }
    }
    return false;
}

Bot &Bot::operator=(const Bot &bot) = default;

Bot::Bot(const Bot &bot) = default;

Bot::~Bot() = default;

}