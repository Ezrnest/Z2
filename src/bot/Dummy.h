//
// Created by liyicheng on 2019/8/10.
//

#ifndef Z2_DUMMY_H
#define Z2_DUMMY_H


#include "Bot.h"

namespace z2 {

class Dummy : public Bot{
public:
protected:

public:
    explicit Dummy(BotDifficulty difficulty = BotDifficulty::NONE);

    void doBotTurn() override;

    const string &getBotName() override;

    ~Dummy() override;
};

}


#endif //Z2_DUMMY_H
