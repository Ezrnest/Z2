/*
 * Created by liyicheng on 2019/7/4.
 */

#ifndef Z2_BUILDING_H
#define Z2_BUILDING_H

#include "EntityWithHealth.h"

namespace z2 {
class Building : public EntityWithHealth{
public:
    explicit Building(int objectId);


    const std::string &getClassName() const override;

    static std::string &className();

    void initialize(const Properties &prop) override;

    static Building *create(int objectId, const Properties &initializer);

protected:
    static void deserializeDataPart(istream &input, Building *en);

    void serializeDataPart(ostream &output) override;

public:

    void serializeTo(ostream &output) override;

    static Building* loadFrom(istream& input);
};

}
#endif //Z2_BUILDING_H
