/*
 * Created by liyicheng on 2019/8/7.
 */

#ifndef Z2_TECHRESEARCH_H
#define Z2_TECHRESEARCH_H

#include "GameMessage.h"
namespace z2{
/**
 * Describes the operation that a player researches a technology..
 * Fields are listed below:
 * <ul>
 * <li><code>playerId</code>: The id of the player.
 * <li><code>techId</code>: The id of the technology that the player want to research.
 * </ul>
 *
 */
class TechResearch : public GameMessage {
private:
    int techId = 0;
    int playerId = 0;
public:
    TechResearch();

    TechResearch(int techId, int playerId);

    int getTechId() const;

    int getPlayerId() const;
    const string &getClassName() const override;
protected:
    void serializeData(ostream &output) override;
public:

    void deserializeData(istream &input) override;


};
}




#endif //Z2_TECHRESEARCH_H
