//
// Created by liyicheng on 2019/8/10.
//

#include "Dummy.h"

namespace z2 {



const string &Dummy::getBotName() {
    static string name = "Dummy";
    return name;
}

void Dummy::doBotTurn() {
    sleepFor(1500);
}

Dummy::Dummy(BotDifficulty difficulty) : Bot(difficulty) {}

Dummy::~Dummy() = default;
}