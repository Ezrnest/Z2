/*
 * Created by liyicheng on 2019/7/4.
 */

#ifndef Z2_PLAYER_H
#define Z2_PLAYER_H

#include <iostream>

using namespace std;
namespace z2 {

class Player {
private:
    string name = "Player";
    /**
     * The id of the player.
     */
    int playerId_ = NO_PLAYER;
    /**
     * The group of the player.
     */
    int groupId_ = NO_GROUP;

    /**
     * The number of gold that this player has.
     */
    int gold_ = 50;

    bool dead = false;


public:
    int getPlayerId() const;

    void setPlayerId(int playerId);

    int getGroupId() const;

    void setGroupId(int groupId);

    int getGold() const;

    void setGold(int gold);

    bool requireGold(int amount);

    void addGold(int delta);

    bool isDead() const;

    void setDead(bool dead = true);

    const string &getName() const;

    void setName(const string &name);

    /**
     * The player id indicating null player.
     */
    static const int NO_PLAYER = -1;
    /**
     * The group id indicating no group, which requires a special process.
     */
    static const int NO_GROUP = -1;

    void saveDataTo(ostream &output);

    void loadDataFrom(istream &input);
};
}

#endif //Z2_PLAYER_H
