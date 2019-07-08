/*
 * Created by liyicheng on 2019/7/4.
 */

#ifndef Z2_FARMER_H
#define Z2_FARMER_H

#include <string>
#include "GameUnit.h"
#include "../world/World.h"

using namespace std;
namespace z2 {
class Farmer : public GameUnit{
private:
    int goldPerTurn = 10;
public:
    explicit Farmer(int objectId);

    const string &identifier() const override;

    static const string& getIdentifier();

    static Entity* create(int objectId, const Properties& initializer);

    void doFarm(const Point& pos, World& world);

    void performAbility(const Point &pos, World &world) override;

    void initialize(const Properties &prop) override;
};

}
#endif //Z2_FARMER_H
