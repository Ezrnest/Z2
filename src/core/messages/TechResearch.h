/*
 * Created by liyicheng on 2019/8/7.
 */

#ifndef Z2_TECHRESEARCH_H
#define Z2_TECHRESEARCH_H

#include "GameMessage.h"
namespace z2{
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
