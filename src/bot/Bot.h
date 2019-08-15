//
// Created by liyicheng on 2019/8/10.
//

#ifndef Z2_BOT_H
#define Z2_BOT_H

#include <ctime>
#include <core/Server.h>
#include <random>
#include "util/BasicUtil.h"
namespace z2 {

enum class BotDifficulty{
    NONE,
    EASY,
    NORMAL,
    HARD
};



class Bot {
private:
public:



    void init(const shared_ptr<Server> &server, const shared_ptr<World> &world, int playerId);

    /**
     * Override this method to customize the bot behavior. To perform an operation,
     * this method should invoke `sendMessage` to send the message of the operation.
     * <br><b>Notice: Direct modification to the server's world will lead to world inconsistency between the server and client! </b>
     * <br>See class `GameMessage` for more info of messages.
     */
    virtual void doBotTurn();

    virtual const string& getBotName() = 0;

    virtual BotDifficulty getDifficulty() =  0;

protected:
    Server* server = nullptr;
    World* world = nullptr;
    int playerId = 0;
    default_random_engine randomEngine { (unsigned int)(ancono::currentTimeMillis())};

    int randomIntBetween(int downerInclusive, int upperExclusive);


    void sleepFor(long millisecond);


    /**
     * Do some initialization. The `server`, `world` is non-null when this method is invoked.
     */
    virtual void doInit();


    /**
     * Calls this function to make an operation.
     */
    void makeOperation(const shared_ptr<GameMessage>& msg);

public:

    Bot();

    Bot(const Bot& bot);

    Bot& operator=(const Bot& bot);

    virtual ~Bot();

};

}


#endif //Z2_BOT_H
