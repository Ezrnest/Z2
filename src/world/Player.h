/*
 * Created by liyicheng on 2019/7/4.
 */

#ifndef Z2_PLAYER_H
#define Z2_PLAYER_H
namespace z2 {

class Player {
private:
    /**
     * The id of the player.
     */
    int playerId_;
    /**
     * The group of the player.
     */
    int groupId_;

    /**
     * The number of gold that this player has.
     */
    int gold_;
public:
    int getPlayerId() const;

    void setPlayerId(int playerId);

    int getGroupId() const;

    void setGroupId(int groupId);

    int getGold() const;

    void setGold(int gold);

};
}

#endif //Z2_PLAYER_H
