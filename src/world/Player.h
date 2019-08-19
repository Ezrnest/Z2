/*
 * Created by liyicheng on 2019/7/4.
 */

#ifndef Z2_PLAYER_H
#define Z2_PLAYER_H

#include <iostream>
#include <set>
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

    int colorCode = 0;


    /**
     * The number of gold that this player has.
     */
    int gold_ = 100;

    bool dead = false;

    /**
     * The remaining number of technologies that this player can research this turn.
     */
    int techPoints = 0;

    int maxTechPoints = 1;

    set<string> technologies;

public:
    int getPlayerId() const;

    void setPlayerId(int playerId);

    int getGroupId() const;

    void setGroupId(int groupId);

    bool isAlly(int group) const;


    int getColorCode() const;

    void setColorCode(int colorCode);

    int getGold() const;

    void setGold(int gold);

    /**
     * Requires the amount of gold of this player. Returns `true` if the player has the
     * required amount of gold, and the gold will be minused, otherwise `false` will be
     * returned and no change will happen.
     */
    bool requireGold(int amount);

    void addGold(int delta);


    bool isDead() const;

    void setDead(bool dead = true);

    const string &getName() const;

    void setName(const string &name);

    int getTechPoints() const;

    void setTechPoints(int techPoints);

    /**
     *
     */
    void consumeTechPoint();

    int getMaxTechPoints() const;

    void setMaxTechPoints(int maxTechPoints);

    void refreshTechPoints();

    const set<string> &getTechnologies() const;

    void addTech(const string& tech);

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
