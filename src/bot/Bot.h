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

enum class BotDifficulty {
    NONE,
    DUMB,
    EASY,
    NORMAL,
    HARD,
};
namespace bot{
class EntityTask;
class TaskMine;
class TaskGuard;
class TaskMoveAttack;
class TaskResearch;
class TaskCaptureResource;
}

class Bot {
private:
public:

    explicit Bot(BotDifficulty difficulty);

    void init(const shared_ptr<Server> &server, const shared_ptr<World> &world, int playerId);

    /**
     * Override this method to customize the bot behavior. To perform an operation,
     * this method should invoke `sendMessage` to send the message of the operation.
     * <br><b>Notice: Direct modification to the server's world will lead to world inconsistency between the server and client! </b>
     * <br>See class `GameMessage` for more info of messages.
     */
    virtual void doBotTurn();

    virtual const string &getBotName() = 0;

    BotDifficulty getDifficulty();

protected:
    shared_ptr<Server> server;
    shared_ptr<World> world;
    int playerId = 0;
    BotDifficulty difficulty = BotDifficulty::NONE;
    default_random_engine randomEngine{(unsigned int) (ancono::currentTimeMillis())};

    int randomIntBetween(int downerInclusive, int upperExclusive);



    void sleepFor(long millisecond);

    /**
     * Do some initialization. The `server`, `world` is non-null when this method is invoked.
     */
    virtual void doInit();

    void setData(int gold, int techPoints);

    /**
     * Calls this function to make an operation.
     */
    virtual void makeOperation(const shared_ptr<GameMessage> &msg);

    shared_ptr<Entity> buyUnit(const Point& pos,const string& name);

    shared_ptr<Entity> buyLatestBattleUnit(const Point &pos, bool buyRangeUnit);


    /**
     * Makes
     */
    bool moveToward(unsigned int entityId, const Point &dest);

    bool clearPos(const Point& pos);

    /**
     * Makes
     */
    virtual bool moveToward(const shared_ptr<Entity>& en, const Point &dest);

    virtual bool attack(const Point& from, const Point& dest);

    virtual void perform(const shared_ptr<Entity>& en);

    const Player& getPlayer();

    vector<shared_ptr<Entity>> getFarmers();

    shared_ptr<Entity> getConstructionBase();

    vector<shared_ptr<Entity>> getAllOwnedEntities();

    vector<shared_ptr<BattleUnit>> getBattleUnits();

    vector<shared_ptr<Entity>> getEnemiesAround(const Point &pos, int d);

    vector<Point> findResources();

    vector<shared_ptr<Entity>> findEnemyEntities(const string& enName);

    vector<shared_ptr<Entity>> findAllEnemies();


public:

    Bot();

    Bot(const Bot &bot);

    Bot &operator=(const Bot &bot);

    virtual ~Bot();


    friend class bot::EntityTask;
    friend class bot::TaskMine;
    friend class bot::TaskGuard;
    friend class bot::TaskMoveAttack;
    friend class bot::TaskResearch;
    friend class bot::TaskCaptureResource;


};

}


#endif //Z2_BOT_H
