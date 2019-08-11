//
// Created by liyicheng on 2019/8/10.
//

#ifndef Z2_SETPLAYERDATA_H
#define Z2_SETPLAYERDATA_H


#include "GameMessage.h"

namespace z2 {
class Player;
class World;


/**
 * Sets one player's game data.
 * Fields are listed below:
 * <ul>
 * <li><code>playerId</code>: The id of the player to set data for.
 * <li><code>gold</code>: The amount of gold to set.
 * <li><code>techPoints</code>: The amount of techPoints to set.
 * <li><code>maxTechPoints</code>: The amount of maxTechPoints to set.
 * </ul>
 */
class SetPlayerData : public GameMessage{
private:
    int playerId = -1;
    int gold = 0;
    int techPoints = 0;
    int maxTechPoints = 0;
private:
    void setDataToPlayer(Player &p);
public:
    explicit SetPlayerData();

    SetPlayerData(int playerId, int gold, int techPoints, int maxTechPoints);

    void setDataForWorld(World& w);

    void deserializeData(istream &input) override;

protected:
    void serializeData(ostream &output) override;

public:
    const string &getClassName() const override;
};

}


#endif //Z2_SETPLAYERDATA_H
