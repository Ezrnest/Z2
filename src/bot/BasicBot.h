/*
 * Created by liyicheng on 2019/8/23.
 */

#ifndef Z2_BASICBOT_H
#define Z2_BASICBOT_H

#include <entity/Farmer.h>
#include "Bot.h"
#include "TaskCaptureResource.h"

namespace z2{
namespace bot{
class BasicBot : public Bot {
protected:
    string name;

    int desiredFarmerCount = 2;
    int minFarmerGuardianCount = 1;
    int maxFarmerGuardianCount = 2;

    int extraGuardianForGem = 2;

    int extraBattleUnitCountToLaunchAttack = 2;



    double goldMultiplier = 2;

    long operationInterval = 100;

    vector<shared_ptr<TaskCaptureResource>> farming;


    vector<shared_ptr<EntityTask>> combating;

    vector<shared_ptr<TaskGuard>> guarding;

    vector<shared_ptr<EntityTask>> extraBattleUnits;

    shared_ptr<TaskResearch> research;

    Point conBase;



    void doInit() override;

    void makeOperation(const shared_ptr<GameMessage> &msg) override;

    void performMine(const Point &pos);

    void perform(const shared_ptr<Entity> &en) override;

    void refreshInfo();


    virtual void doFarm();

    void launchAttackTo(EntityId enemy, int maxCount);

    bool launchAttackToFarmer();

    bool launchAttackToBase();

    bool launchAttackToAll();

    /**
     * Returns `true` if a new resource is found and entities are prepared, which implies that `farming.size()` is
     * increased.
     */
    virtual bool planForNewResource();

    virtual bool strengthenMilitaryForce();

    virtual void consolidateFarming(const shared_ptr<Entity> &en);

    virtual void doCombat();

    virtual void doCommandUnits();

    virtual void doBasicProduction();

    virtual void doExtraProduction();

    virtual void doResearch();

    Point findNextNearestResource();

    Point findRandomEmptyTile(int d=1);
public:
    void doBotTurn() override;

    const string &getBotName() override;

public:
    BasicBot(BotDifficulty difficulty, const string &name);

//    static BotProvider
    friend class BasicBotProvider;
};

class BasicBotProvider{
private:
    double goldMultiplier;
    string name;
public:
    BasicBotProvider(double goldMultiplier, const string &name);

    shared_ptr<Bot> operator()(BotDifficulty difficulty);
};

}
}



#endif //Z2_BASICBOT_H
