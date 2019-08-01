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
    explicit Farmer(unsigned int objectId);

    const string &getClassName() const override;

    static const string& className();

    static Entity* create(int objectId, const Properties& initializer);

    void doFarm(const Point& pos, World& world);

    void performAbility(const Point &pos, World &world) override;

    bool canPerformAbility(const Point &pos, World &world) override;

    void initialize(const Properties &prop) override;

protected:
    void serializeDataPart(ostream &output) override;

    static void deserializeDataPart(istream &input, Farmer* en);
public:
    void serializeTo(ostream &output) override;

    static Farmer* loadFrom(istream& input);
};

}
#endif //Z2_FARMER_H
