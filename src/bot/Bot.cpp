//
// Created by liyicheng on 2019/8/10.
//

#include "Bot.h"
#include <chrono>
#include <thread>



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
    if(server){
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

Bot::Bot(){
    randomEngine();
}

Bot &Bot::operator=(const Bot &bot) = default;

Bot::Bot(const Bot &bot) = default;

Bot::~Bot() = default;

}